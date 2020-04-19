#include <bits/stdc++.h>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>

using namespace std;

int XSize=145,Order_number;
//XSize is the screen width in spaces, Order_number stores order number in a shop

void printtocenter(string s,int i){
    for(int ii=0;ii<((XSize-s.length()-1)/2);ii++)cout<<" ";
    if(i==1)cout<<s<<endl;
    if(i==2)cout<<s;
}
//printtocenter prints string s to the center of the screen, i denotes which line to print in

class Credentials{
private:
    string username,password;
    vector<string> Usernames,Passwords;
    //These data members can not be kept public
    #define KEY "zE20Ar15ckS"
    //Key is used for encryption of passwords

public:

    Credentials(string x,string y){
        username=x;
        password=y;
    }
    //username and password in the current instance are initialized in constructor

    void Init(){
        ifstream fin;
        string buffer;
        Usernames.clear();
        Passwords.clear();
        fin.open("Credentials.txt");
        int i;
        fin>>i;
        for(int ii=0;ii<i;ii++){
            fin>>buffer;
            Usernames.push_back(buffer);
            fin>>buffer;
            Passwords.push_back(buffer);
        }
        fin.close();
        password=encrypt(password);
    }
    //All passwords and usernames are stored into respective vectors

    string encrypt(string pass){
        for(int ii=0;ii<pass.size();ii++){
            pass[ii]=((pass[ii]+KEY[ii%11])%26)+'a';
        }
        return pass;
    }
    //Used to encrypt input password

    void Uninit(){
        ofstream fout;
        string buffer;
        fout.open("Credentials.txt");
        fout<<Usernames.size()<<endl;
        for(int ii=0;ii<Usernames.size();ii++){
            fout<<Usernames[ii]<<endl<<Passwords[ii]<<endl;
        }
        fout.close();
    }
    //All usernames and passwords are written back into the file

    int Validate(){
        int i;
        for(i=0;i<Usernames.size();i++){
            if(Usernames[i]==username)break;
        }
        if(i==Usernames.size())return -1;
        if(Passwords[i]==password)return i;
        else return -2;
    }
    //Validate checks whether input username and password correspond to a cashier/shop

    int add(string usern,string passw){
        Usernames.push_back(usern);
        Passwords.push_back(encrypt(passw));
        this->Uninit();
        return Usernames.size()-1;
    }
    //add can add a new shop to the existing list

};
//Credentials handles encryption, checking if username and password are correct and adding new credential item.

class Item{
public:
    string item_name;
    unsigned int cost,stock;
    Item(string x,int y,int z){
        item_name=x;
        cost=y;
        stock=z;
    }
    //Initializes item_name, cost and stock of item.

};
//Each Item in a shop has an instance of Item class

class Order{
public:
    int token_number,Shop_id;
    vector<string> name;
    vector<int> cost;
    vector<int> quantity;

    Order(int i,int id,vector<string> S,vector<int> q,vector<int> c){
        token_number=i;
        Shop_id=id;
        name=S;
        cost=c;
        quantity=q;
    }
    //Initializes token_number, shop_id, name, cost and quantity

};
//Each Order made at the cashier has an instance of Order class

class Shop{
private:
    string Shop_name,Shop_owner,pending_name="_shop_pend.txt",menu_name="_shop_menu.txt";
    ifstream mi,pi;
    ofstream mo,po;
    int id;
    vector<Item*> menu;
    vector<Order*> orders;
public:
    Shop(int i){
        id=i;
        string qw=to_string(i);
        pending_name=qw+pending_name;
        menu_name=qw+menu_name;
        int k,j;
        fstream fi;
        fi.open("status.txt",ios::in);
        fi>>k>>j;
        fi.close();
        fi.open("status.txt",ios::out);
        fi<<++k<<endl<<j;
        fi.close();
    }
    //i denotes which shop instance this is.

    void Init(int i=0){
        orders.clear();
        menu.clear();
        string buffer;
        pi.open(pending_name);
        int num;
        pi>>num;
        for(int ii=0;ii<num;ii++){
            if(pi.eof())break;
            vector<string> a;
            int i,d,b1,c1;
            string a1;
            vector<int> b,c;
            pi>>i>>d;
            for(int ii=0;ii<d;ii++){
                pi>>a1>>b1>>c1;
                a.push_back(a1);
                b.push_back(b1);
                c.push_back(c1);
            }
            Order* temp=new Order(i,id,a,b,c);
            orders.push_back(temp);
        }
        pi.close();
        mi.open(menu_name);
        mi>>Shop_name;
        mi>>Shop_owner;
        int item_num;
        mi>>item_num;
        for(int ii=0;ii<item_num;ii++){
            string a;
            int b,c;
            mi>>a>>b>>c;        /// a-Item name, b-cost, c-quantity
            Item* temp=new Item(a,b,c);
            menu.push_back(temp);
        }
        mi.close();
        if(i==1)this->Start();
        return;
    }
    //All the menu items and previous orders are input into vectors.

    void Start(){
        char c='\0';
        while(1){
            system("reset");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("************************",1);
            printtocenter("***** "+Shop_name+" *****",1);
            printtocenter("************************",1);
            cout<<endl;
            printtocenter("1. View Pending Orders",1);
            cout<<endl;
            printtocenter("2. Clear Pending Order",1);
            cout<<endl;
            printtocenter("3. Add New Item",1);
            cout<<endl;
            printtocenter("4. Edit Existing Item",1);
            cout<<endl;
            printtocenter("5. Log Out",1);
            cout<<endl;
            printtocenter("Enter appropriate digit : ",2);
            cin>>c;
            if(c!='1'&&c!='2'&&c!='3'&&c!='4'&&c!='5'){
                continue;
            }
            if(c=='5'){
                this->Uninit();
                break;
            }
            else if(c=='1'){
                this->Pending_Orders();
            }
            else if(c=='3'){
                this->new_item();
            }
            else if(c=='4'){
                this->existing_item();
            }
            else if(c=='2'){
                this->clear_order();
            }
        }
        return;
    }
    //Shop commands are printed and respective methods are called.

    void Uninit(){
        ofstream f;
        f.open(menu_name);
        f<<Shop_name<<endl;
        f<<Shop_owner<<endl<<menu.size();
        for(int ii=0;ii<menu.size();ii++){
            f<<endl<<menu[ii]->item_name<<endl<<menu[ii]->cost<<endl<<menu[ii]->stock;
        }
        f.close();
        f.open(pending_name);
        f<<orders.size()<<endl;
        for(int ii=0;ii<orders.size();ii++){
            f<<orders[ii]->token_number<<endl<<orders[ii]->cost.size()<<endl;
            for(int jj=0;jj<orders[ii]->cost.size();jj++){
                f<<orders[ii]->name[jj]<<endl<<orders[ii]->quantity[jj]<<endl<<orders[ii]->cost[jj]<<endl;
            }
        }
        f.close();
        int k,j;
        fstream fi;
        fi.open("status.txt",ios::in);
        fi>>k>>j;
        fi.close();
        fi.open("status.txt",ios::out);
        fi<<--k<<endl<<j;
        fi.close();
        return;
    }
    //All menu items and previous orders are output into respective files

    void clear_order(){
        system("clear");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Enter the token number to clear order",1);
        printtocenter("Press x to go back",1);
        printtocenter("",2);
        string j;
        cin>>j;
        if(j=="x")return;
        int i=0,k=0;
        for(int ii=0;ii<j.size();ii++){
            i=i*10+j[ii]-'0';
        }
        this->Init();
        int t=orders.size();
        for(k=0;k<orders.size();k++){
            if(orders[k]->token_number==i){
                orders.erase(orders.begin()+k);
                break;
            }
        }
        if(k==t){
            system("clear");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Order not found!",2);
            usleep(1000*3000);
            this->clear_order();
            return;
        }
        this->Uninit();
        return;
    }
    // A particular order from the existing orders can be removed if token number is known.

    void existing_item(){
        system("clear");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        char c='\0';
        printtocenter("Do you want to edit a existing Menu item?(y/n) ",2);
        while(c!='n'&&c!='y')cin>>c;
        if(c=='n')return;
        system("clear");
        cout<<endl;
        printtocenter("*** "+Shop_name+" ***",1);
        printtocenter("Owner : "+Shop_owner,1);
        cout<<endl<<endl;
        this->Init();
        if(menu.size()==0){
            cout<<endl<<endl;
            printtocenter("*** No Items in Menu ***",1);
            usleep(1000*3000);
            return;
        }
        printtocenter("****  Menu  ****",1);
        cout<<endl<<setw(15)<<"S.No"<<setw(30)<<"Item Name"<<setw(35)<<"Quantity in Stock"<<setw(20)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<menu.size();ii++){
            cout<<setw(15)<<ii+1<<setw(30)<<menu[ii]->item_name<<setw(28)<<menu[ii]->stock<<setw(26)<<menu[ii]->cost<<endl<<endl;
        }
        cout<<endl;
        printtocenter("Enter Serial Number of Item : ",2);
        string m;
        cin>>m;
        int k=0;
        for(int ii=0;ii<m.size();ii++){
            k=k*10+m[ii]-'0';
        }
        cout<<endl<<endl;
        if(k<=0||k>menu.size()){
            printtocenter("Invalid Serial Number!",2);
            usleep(1000*2000);
            this->existing_item();
            return;
        }
        c='\0';
        printtocenter("Do you want to edit or delete item?(e/d) ",2);
        while(c!='e'&&c!='d')cin>>c;
        if(c=='d'){
            menu.erase(menu.begin()+k-1);
        }else{
            system("clear");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            string s;
            int a,b;
            printtocenter("Current Item Name : "+menu[k-1]->item_name,2);
            cout<<endl;
            printtocenter("Enter New Name : ",2);
            cin>>s;
            cout<<endl<<endl;
            printtocenter("Current Item Cost : ",2);
            cout<<menu[k-1]->cost<<endl;
            printtocenter("Enter New Cost : ",2);
            cin>>a;
            cout<<endl<<endl;
            printtocenter("Current Item Remaining Quantity : ",2);
            cout<<menu[k-1]->stock<<endl;
            printtocenter("Enter New Quantity : ",2);
            cin>>b;
            cout<<endl<<endl;
            printtocenter("Confirm changes?(y/n) ",2);
            c='\0';
            while(c!='y'&&c!='n')cin>>c;
            if(c=='n'){
                this->existing_item();
                return;
            }
            menu[k-1]->item_name=s;
            menu[k-1]->cost=a;
            menu[k-1]->stock=b;
        }
        this->Uninit();
        return;
    }
    //Edit/Delete existing items

    void Pending_Orders(){
        system("clear");
        cout<<endl;
        printtocenter("*** "+Shop_name+" ***",1);
        printtocenter("Owner : "+Shop_owner,1);
        cout<<endl<<endl;
        this->Init();
        if(orders.size()==0){
            cout<<endl<<endl;
            printtocenter("*** No Orders Pending ***",1);
            usleep(1000*2000);
            return;
        }
        printtocenter("*** Pending Orders ***",1);
        cout<<endl<<setw(20)<<"Token No"<<setw(30)<<"Item Name"<<setw(20)<<"Quantity"<<setw(20)<<"Cost"<<endl<<endl<<endl;
        int ii;
        for(int ii=0;ii<orders.size();ii++){
            cout<<setw(18)<<orders[ii]->token_number;
            int co=0;
            for(int jj=0;jj<(orders[ii]->cost).size();jj++){
                cout<<setw((jj==0)?32:50)<<orders[ii]->name[jj]<<setw(17)<<orders[ii]->quantity[jj]<<setw(23)<<orders[ii]->cost[jj]<<endl;
                co+=orders[ii]->cost[jj];
            }
            cout<<setw(90)<<co<<endl<<endl<<endl;
        }
        printtocenter("Enter r to refresh",1);
        printtocenter("Enter x to go back ",2);
        char c='\0';
        while(c!='x'&&c!='r')cin>>c;
        if(c=='x'){
            return;
        }
        else if(c=='r'){
            this->Pending_Orders();
            return;
        }
    }
    //List of all pending orders is displayed

    void new_item(){
        system("clear");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Do you want to add a new item?(y/n) ",2);
        char c='\0';
        while(c!='y'&&c!='n')cin>>c;
        if(c=='n')return;
        if(c=='y'){
            system("clear");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Enter Item name : ",2);
            string s;
            int c,q;
            cin>>s;
            cout<<"\n";
            printtocenter("Enter cost : ",2);
            cin>>c;
            cout<<"\n";
            printtocenter("Enter quantity available : ",2);
            cin>>q;
            cout<<"\n";
            printtocenter("Confirm details?(y/n) ",2);
            char d='\0';
            while(d!='y'&&d!='n')cin>>d;
            if(d=='n'){
                this->new_item();
                return;
            }else{
                this->Init();
                for(int ii=0;ii<menu.size();ii++){
                    if(menu[ii]->item_name==s){
                        system("clear");
                        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
                        printtocenter("Item already present in the Menu!",2);
                        char e;
                        cin>>e;
                        this->new_item();
                        return;
                    }
                }
                Item* temp=new Item(s,c,q);
                menu.push_back(temp);
                this->Uninit();
                return;
            }
        }
    }
    //Adding new items to the menu

};
//this class handles all methods required for shop accounts

class Cashier{
private:
    const string pending_name="_shop_pend.txt",menu_name="_shop_menu.txt";
    vector<string> shop_names,shop_names2,menu2;
    vector<bool> sel;
    vector<Item*> menu,cart;
    Item* temp;
public:
    Cashier(){}
    //no specific commands in the constructor
    void Init(string si){
        menu.clear();
        menu2.clear();
        int item_num;
        ifstream mi;
        mi.open(si);
        mi>>si;
        mi>>si;
        mi>>item_num;
        for(int ii=0;ii<item_num;ii++){
            string a;
            int b,c;
            mi>>a>>b>>c;        /// a-Item name, b-cost, c-quantity
            Item* temp=new Item(a,b,c);
            menu.push_back(temp);
            transform(a.begin(),a.end(),a.begin(),::tolower);
            menu2.push_back(a);
        }
        mi.close();
    }
    //All the menu items of shop si are loaded into menu vector

    void refresh(){
        shop_names.clear();
        shop_names2.clear();
        shop_names.push_back("");
        shop_names2.push_back("");
        ifstream fin;
        fin.open("Credentials.txt");
        int i;
        fin>>i;
        fin.close();
        string x1;
        for(int ii=1;ii<i;ii++){
            string x2;
            x1=to_string(ii);
            x1=x1+"_shop_menu.txt";
            fin.open(x1);
            fin>>x2;
            fin.close();
            shop_names.push_back(x2);
            transform(x2.begin(),x2.end(),x2.begin(),::tolower);
            shop_names2.push_back(x2);
        }
        sel.resize(shop_names.size());
        return;
    }
    //In case of deletion or addition of shops, vectors are refreshed.

    void Start(){
        while(1){
            system("reset");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("1. Make a New Order",1);
            cout<<endl;
            printtocenter("2. View Previous Orders",1);
            cout<<endl;
            printtocenter("3. View Menus",1);
            cout<<endl;
            printtocenter("4. Edit Existing Shops",1);
            cout<<endl;
            printtocenter("5. Log Out",1);
            cout<<endl;
            printtocenter("Enter appropriate digit : ",2);
            char c='\0';
            while(c!='1'&&c!='2'&&c!='3'&&c!='4'&&c!='5')cin>>c;
            if(c=='1'){
                this->new_order();
            }
            if(c=='2'){
                this->prev_orders();
            }
            if(c=='3'){
                this->view_menu();
            }
            if(c=='4'){
                this->edit_shops();
            }
            if(c=='5'){
                fstream fi;
                fi.open("status.txt",ios::out);
                fi<<0<<endl<<Order_number;
                fi.close();
                break;
            }
        }
        return;
    }
    //Start is the first method called after constructor
    //Cashier commands are printed and respective methods are called until logout

    void new_order(){
        cart.clear();
        this->refresh();
        int k=this->shop_sel('a');              //shop_sel handles selection of shop to place a order
        if(k==-1)return;
        string k1=to_string(k),k2=to_string(k);
        k1=k1+"_shop_menu.txt";
        k2=k2+"_shop_pend.txt";
        this->Init(k1);                         //Init initializes the menu vector with menu of shop k1
        char c='\0';
        while(1){
            system("clear");
            cout<<endl<<endl<<endl;
            this->pr_cart();                    //Selected items are printed
            cout<<endl<<endl<<endl;
            printtocenter("Enter a to Add New Item",1);
            printtocenter("Enter f to Finish Order",1);
            printtocenter("Enter x to go back",1);
            printtocenter("",2);
            c='\0';
            while(c!='a'&&c!='f'&&c!='x')cin>>c;
            if(c=='x'||c=='f')break;
            sel.resize(menu.size());
            int q=this->sel_item('a');          //sel_item handles selection of an item from the menu
            if(q==-1)break;
            system("clear");
            cout<<endl<<endl<<endl;
            printtocenter("How many "+menu[q]->item_name+" do you want? ",2);
            int qua=0;
            cin>>qua;
            while(qua>menu[q]->stock){
                cout<<endl<<endl;
                printtocenter("Available Stock : "+to_string(menu[q]->stock),1);
                cout<<endl;
                printtocenter("How many "+menu[q]->item_name+" do you want? ",2);
                cin>>qua;
            }
            cout<<endl<<endl;
            printtocenter("Confirm Item?(y/n) ",2);
            char f='\0';
            while(f!='n'&&f!='y')cin>>f;
            if(f=='n')continue;
            menu[q]->stock-=qua;
            temp=new Item(menu[q]->item_name,(menu[q]->cost)*qua,qua);
            cart.push_back(temp);
        }
        if(c=='x')return;
        if(cart.size()==0)return;
        system("reset");
        cout<<endl<<endl<<endl;
        printtocenter("Order placed Successfully",1);
        cout<<endl<<endl<<endl;
        printtocenter("Token Number : "+to_string(Order_number),1);
        cout<<endl<<endl<<endl;
        this->pr_cart();
        cout<<endl<<endl<<endl;
        printtocenter("Enter x to go back  ",2);
        char g='\0';
        while(g!='x')cin>>g;
        fstream fi;
        fi.open(k2,ios::in|ios::binary|ios::out);
        int num,pos;
        pos=fi.tellg();
        fi>>num;
        fi.seekp(pos);
        fi<<num+1;
        fi.close();
        fi.open(k2,ios::app);
        fi<<Order_number<<endl<<cart.size()<<endl;
        for(int ii=0;ii<cart.size();ii++){
            fi<<cart[ii]->item_name<<endl<<cart[ii]->stock<<endl<<cart[ii]->cost<<endl;
        }
        fi.close();
        k2="prev_order.txt";
        fi.open(k2,ios::in|ios::binary|ios::out);
        pos=fi.tellg();
        fi>>num;
        fi.seekp(pos);
        fi<<num+1;                                          //new order is added into the shop's pending orders list
        fi.close();
        fi.open(k2,ios::app);
        fi<<Order_number<<endl<<k<<endl<<cart.size()<<endl;
        Order_number++;
        for(int ii=0;ii<cart.size();ii++){
            fi<<cart[ii]->item_name<<endl<<cart[ii]->stock<<endl<<cart[ii]->cost<<endl;
        }
        fi.close();
        fi.open(k1,ios::in);
        string s1,s2;
        fi>>s1>>s2;
        fi.close();
        fi.open(k1,ios::out);
        fi<<s1<<endl<<s2<<endl<<menu.size()<<endl;
        for(int ii=0;ii<menu.size();ii++){
            fi<<menu[ii]->item_name<<endl<<menu[ii]->cost<<endl<<menu[ii]->stock<<endl;
        }
        fi.close();
    }
    //Placing new orders is handled

    int sel_item(char d){
        system("clear");
        cout<<endl<<endl;
        printtocenter("Select an Item",1);
        cout<<endl<<endl;
        for(int ii=0;ii<sel.size();ii++)sel[ii]=true;
        int q=pr_items("");
        if(q==0){
            printtocenter("No available items in the shop",1);
            usleep(1000*2000);
            return -1;
        }
        if(d!='e'&&d!='s'&&d!='x')printtocenter("Enter e to enter index,s to search or x to go back(e/s/x) ",2);
        char c=d;
        while(c!='s'&&c!='e'&&c!='x')cin>>c;
        if(c=='x')return -1;
        if(c=='s'){
            cout<<endl<<endl<<endl;
            printtocenter("Enter Search key : ",2);
            string sea;
            cin>>sea;
            system("clear");
            cout<<endl<<endl;
            printtocenter("Select an Item",1);
            cout<<endl<<endl;
            for(int ii=0;ii<sel.size();ii++)sel[ii]=true;
            pr_items(sea);
            cout<<endl;
            printtocenter("Enter e to enter index,s to search or x to go back(e/s/x) ",2);
            char e='\0';
            while(e!='x'&&e!='s'&&e!='e')cin>>e;
            if(e=='x')return -1;
            if(e=='s')return this->sel_item('s');
        }
        cout<<endl<<endl;
        printtocenter("Enter the Serial no. of the Item : ",2);
        int p;
        cin>>p;
        p--;
        if(p<0||p>=menu.size()){
            cout<<endl<<endl;
            printtocenter("Invalid Serial no!",2);
            usleep(1000*2000);
            return this->sel_item('a');
        }
        if(sel[p]==false){
            cout<<endl<<endl;
            printtocenter("Serial no. not among Search results!",2);
            usleep(1000*2000);
            return this->sel_item('a');
        }
        return p;
    }
    //Selection of an item from the menu is handled here.

    int pr_items(string s){
        if(s!=""){
            transform(s.begin(),s.end(),s.begin(),::tolower);
            for(int ii=0;ii<menu.size();ii++){
                if(this->isss(s,menu2[ii],s.size(),menu2[ii].size())==false)sel[ii]=false;
            }
        }
        int cou=0;
        for(int ii=0;ii<sel.size();ii++)if(sel[ii]&&menu[ii]->stock!=0)cou++;
        if(cou==0){
            printtocenter("No items found!",1);
            return 0;
        }
        cout<<setw(30)<<"S.No"<<setw(30)<<"Item Name"<<setw(25)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<menu.size();ii++){
            if(sel[ii]==true&&menu[ii]->stock!=0){
                cout<<setw(30)<<ii+1<<setw(30)<<menu[ii]->item_name<<setw(24)<<menu[ii]->cost<<endl<<endl;
            }
        }
        return cou;
    }
    //All items in the menu containing subsequence s are printed

    void pr_cart(){
        if(cart.size()==0){
            printtocenter("Cart is Empty!",1);
            return;
        }
        printtocenter("****  Cart  ****",1);
        cout<<endl<<endl;
        cout<<setw(15)<<"S.No"<<setw(30)<<"Item Name"<<setw(35)<<"Quantity"<<setw(20)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<cart.size();ii++){
            cout<<setw(15)<<ii+1<<setw(30)<<cart[ii]->item_name<<setw(28)<<cart[ii]->stock<<setw(26)<<cart[ii]->cost<<endl<<endl;
        }
        cout<<endl;
    }
    //all items in the cart are printed

    void prev_orders(){
        system("reset");
        ifstream fin;
        fin.open("prev_order.txt");
        int k;
        fin>>k;
        this->refresh();
        cout<<endl<<endl;
        if(k==0){
            cout<<endl;
            printtocenter("No record of Previous Orders!",1);
            usleep(1000*2500);
            return;
        }
        printtocenter("*************************",1);
        printtocenter("***  Previous Orders  ***",1);
        printtocenter("*************************",1);
        cout<<endl<<endl;
        cout<<setw(15)<<"Shop Name"<<setw(25)<<"Token No"<<setw(30)<<"Item Name"<<setw(25)<<"Quantity"<<setw(20)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<k;ii++){
            int a1,b1,c1,i1=0;
            vector<string> e1;
            vector<int> f1,g1;
            fin>>a1>>b1>>c1;
            for(int jj=0;jj<c1;jj++){
                string s;
                int d1,h1;
                fin>>s>>d1>>h1;
                e1.push_back(s);        ///token no,shop no,no.of items,name,quantity,cost
                f1.push_back(d1);
                g1.push_back(h1);
            }
            cout<<setw(15)<<shop_names[b1]<<setw(22)<<a1<<setw(33)<<e1[0]<<setw(22)<<f1[0]<<setw(23)<<g1[0]<<endl;
            i1+=g1[0];
            for(int jj=1;jj<c1;jj++){
                cout<<setw(70)<<e1[jj]<<setw(22)<<f1[jj]<<setw(23)<<g1[jj]<<endl;
                i1+=g1[jj];
            }
            cout<<setw(115)<<i1<<endl<<endl<<endl;
        }
        fin.close();
        printtocenter("Enter x to go back ",2);
        char c='\0';
        while(c!='x')cin>>c;
        return;
    }
    //A list of all previous orders is printed. When a shop is deleted, the entire list is cleared.

    int shop_sel(char d){
        system("clear");
        cout<<endl<<endl;
        printtocenter("Select a Shop",1);
        cout<<endl<<endl;
        for(int ii=0;ii<sel.size();ii++)sel[ii]=true;
        pr_shops("");
        if(d!='e'&&d!='s'&&d!='x')printtocenter("Enter e to enter index,s to search or x to go back(e/s/x) ",2);
        char c=d;
        while(c!='s'&&c!='e'&&c!='x')cin>>c;
        if(c=='x')return -1;
        if(c=='s'){
            cout<<endl<<endl<<endl;
            printtocenter("Enter Search key : ",2);
            string sea;
            cin>>sea;
            system("clear");
            cout<<endl<<endl;
            printtocenter("Select a Shop",1);
            cout<<endl<<endl;
            for(int ii=0;ii<sel.size();ii++)sel[ii]=true;
            pr_shops(sea);
            cout<<endl;
            printtocenter("Enter e to enter index,s to search or x to go back(e/s/x) ",2);
            char e='\0';
            while(e!='x'&&e!='s'&&e!='e')cin>>e;
            if(e=='x')return -1;
            if(e=='s')return this->shop_sel('s');
        }
        cout<<endl<<endl;
        printtocenter("Enter the Serial no. of the Shop : ",2);
        int p;
        cin>>p;
        if(p<=0||p>=shop_names.size()){
            cout<<endl<<endl;
            printtocenter("Invalid Serial no!",2);
            usleep(1000*2000);
            return this->shop_sel('a');
        }
        if(sel[p]==false){
            cout<<endl<<endl;
            printtocenter("Serial no. not among Search results!",2);
            usleep(1000*2000);
            return this->shop_sel('a');
        }
        return p;
    }
    //Selecting a shop is handled

    void pr_shops(string s){
        if(s!=""){
            transform(s.begin(),s.end(),s.begin(),::tolower);
            for(int ii=1;ii<shop_names.size();ii++){
                if(this->isss(s,shop_names2[ii],s.size(),shop_names2[ii].size())==false)sel[ii]=false;
            }
        }
        int cou=0;
        for(int ii=1;ii<shop_names.size();ii++){
            if(sel[ii]==true){
                cout<<setw(70)<<ii<<"\t"<<shop_names[ii]<<endl<<endl;
                cou++;
            }
        }
        if(cou==0){
            printtocenter("No matches found!",1);
        }
    }
    //All shops are printed containing subsequence s

    bool isss(string str1,string str2,int m,int n){
        if(m==0)return true;
        if(n==0)return false;
        if(str1[m-1]==str2[n-1])return this->isss(str1,str2,m-1,n-1);
        return this->isss(str1,str2,m,n-1);
    }
    //checks if str1 is a sub sequence string of str2

    void view_menu(){
        this->refresh();
        int k=this->shop_sel('a');
        if(k==-1)return;
        ifstream fin;
        string k1=to_string(k);
        k1=k1+"_shop_menu.txt";
        this->Init(k1);
        fin.open(k1);
        system("clear");
        cout<<endl<<endl;
        fin>>k1;
        printtocenter("*** "+k1+" ***",1);
        fin>>k1;
        printtocenter("Shop Owner : "+k1,1);
        fin.close();
        cout<<endl<<endl;
        if(menu.size()==0){
            cout<<endl<<endl;
            printtocenter("*** No Items in Menu ***",1);
            usleep(1000*3000);
            return;
        }
        printtocenter("****  Menu  ****",1);
        cout<<endl<<setw(15)<<"S.No"<<setw(30)<<"Item Name"<<setw(35)<<"Quantity in Stock"<<setw(20)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<menu.size();ii++){
            cout<<setw(15)<<ii+1<<setw(30)<<menu[ii]->item_name<<setw(28)<<menu[ii]->stock<<setw(26)<<menu[ii]->cost<<endl<<endl;
        }
        cout<<endl;
        printtocenter("Enter x to go back ",2);
        char f='\0';
        while(f!='x')cin>>f;
        return;
    }
    //All menu items of a shop are printed

    void edit_shops(){
        system("reset");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Do you want to add or remove an existing shop?(a/r) ",1);
        printtocenter("Enter x to go back",1);
        printtocenter("",2);
        char c='\0';
        while(c!='x'&&c!='r'&&c!='a')cin>>c;
        if(c=='x')return;
        else{
            system("clear");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";;
            printtocenter("Enter root password to proceed : ",2);
            string pas;
            char c='\0';
            cin>>pas;
            Credentials D("master",pas);
            D.Init();
            int u=D.Validate();
            if(u!=0){
                cout<<endl<<endl;
                printtocenter("Wrong Password!",1);
                usleep(1000*2000);
                return;
            }
        }
        if(c=='r'){
            this->refresh();
            system("reset");
            cout<<"\n\n\n";
            if(shop_names.size()==1){
                printtocenter("No Existing shops to remove!",2);
                usleep(1000*3000);
                return;
            }
            printtocenter("***  Existing Shops  ***",1);
            cout<<"\n\n";
            for(int ii=1;ii<shop_names.size();ii++){
                cout<<setw(60)<<ii<<"\t"<<shop_names[ii]<<endl;
            }
            cout<<endl<<endl;
            printtocenter("Enter the Serial No. of the shop you want to remove : ",2);
            int k=0;
            cin>>k;
            if(k<=0||k>shop_names.size()){
                cout<<endl;
                printtocenter("Invalid Serial No!",1);
                usleep(1000*3000);
                return;
            }
            fstream fi1;
            fi1.open("status.txt",ios::in);
            int status;
            fi1>>status;
            fi1.close();
            if(status!=0){
                system("reset");
                cout<<endl<<endl<<endl<<endl;
                printtocenter("All Shops must be closed while removing a shop",1);
                printtocenter("Could not complete removal operation due to open shops!!!",1);
                usleep(1000*3500);
                return;
            }
            ofstream f;
            f.open("prev_order.txt");
            f<<0<<endl;
            f.close();
            ifstream fi;
            fi.open("Credentials.txt");
            int m;
            fi>>m;
            vector<string> a,b;
            string tem1,tem2;
            for(int ii=0;ii<m;ii++){
                fi>>tem1;
                fi>>tem2;
                if(ii==k)continue;
                a.push_back(tem1);
                b.push_back(tem2);
            }
            fi.close();
            f.open("Credentials.txt");
            f<<a.size()<<endl;
            for(int ii=0;ii<a.size();ii++){
                f<<a[ii]<<endl<<b[ii]<<endl;
            }
            f.close();
            string x1,x2,x3,x4;
            x1=to_string(k);
            x2=x1+"_shop_pend.txt";
            remove(x2.c_str());
            x2=x1+"_shop_menu.txt";
            remove(x2.c_str());
            for(int ii=k;ii<a.size();ii++){
                x1=to_string(ii);
                x2=to_string(ii+1);
                x3=x1+"_shop_menu.txt";
                x4=x2+"_shop_menu.txt";
                rename(x4.c_str(),x3.c_str());
                x3=x1+"_shop_pend.txt";
                x4=x2+"_shop_pend.txt";
                rename(x4.c_str(),x3.c_str());
            }
        }else{
            system("reset");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Enter Shop Name : ",2);
            string name,owner,usern,passw;
            cin>>name;
            cout<<endl<<endl;
            printtocenter("Enter Owner Name : ",2);
            cin>>owner;
            cout<<endl<<endl;
            printtocenter("Enter Username : ",2);
            cin>>usern;
            cout<<endl<<endl;
            printtocenter("Enter Password : ",2);
            cin>>passw;
            cout<<endl<<endl;
            printtocenter("Confirm Details?(y/n) ",2);
            char c='\0';
            while(c!='n'&&c!='y')cin>>c;
            if(c=='n')return;
            Credentials D("abc","abc");
            D.Init();
            int ind=D.add(usern,passw);
            string x5=to_string(ind),x6;
            x6=x5+"_shop_menu.txt";
            ofstream fout1;
            fout1.open(x6);
            fout1<<name<<endl<<owner<<endl<<"0";
            fout1.close();
            x6=x5+"_shop_pend.txt";
            fout1.open(x6);
            fout1<<"0";
            fout1.close();
            return;
        }
    }
    //Addition of deletion of existing shops is handled

};
//This class handles all the methods required for cashier account.

class Print{
private:
    string password,username;
public:
    Print(){
        system("clear");
        string Welcome_string="Welcome to Canteen Management Portal...";
        cout.flush();
        usleep(1000*400);
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t";
        for(int ii=0;Welcome_string[ii]!='\0';ii++){
            cout<<Welcome_string[ii];
            cout.flush();
            usleep(1000*50);
        }
        usleep(1000*400);
        for(int ii=0;ii<Welcome_string.length();ii++){
            cout<<"\b \b";
            cout.flush();
            usleep(1000*25);
        }
    }
    //Welcome message is printed

    void Start(){
        system("reset");
        char c;
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("1. Login",1);
        cout<<endl;
        printtocenter("2. Exit",1);
        cout<<endl;
        printtocenter("Enter appropriate digit : ",2);
        cin>>c;
        while(c!='1'&&c!='2'){
            this->Start();
            return;
        }
        if(c=='1'){
            this->Login();
        }
        return;
    }
    //Login/Exit is handled on entry. Returns here on any logout.

    void Login(){
        system("clear");
        string pw_stars;
        char c;
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Enter Username : ",2);
        cin>>username;
        password.clear();
        system("clear");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Enter Password : ",2);
        cin>>password;
        system("clear");
        Credentials C(username,password);
        C.Init();
        int i=C.Validate();     //Username and Password are validated
        if(i==-1){
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Username not found!!!",1);
            usleep(1000*2000);
            this->Start();
            return;
        }                       //Username not found case
        if(i==-2){
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Wrong Password!!!",1);
            usleep(1000*2000);
            this->Start();
            return;
        }                        //Wrong password case
        if(i==0){
            Cashier C;
            C.Start();
        }else{
            Shop shop(i);
            shop.Init(1);
        }
        this->Start();
        return;
    }
    //Login handles username and password input and creates an instance of respective shop, returns to start on logout.

    void End(){
        system("clear");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Thank you",1);
        usleep(1000*2000);
        system("reset");
        return;
    }
    //End prints thank you message

};
//This class handles the procedure that is common to both cashier and the shops.

int main(){
    int i;
    ifstream fi;
    fi.open("status.txt",ios::in);
    fi>>i;
    fi>>i;
    fi.close();
    Order_number=i;
    Print P;
    P.Start();
    P.End();
    return 0;
}
