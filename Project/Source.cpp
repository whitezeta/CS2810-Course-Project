#include <conio.h>
#include <windows.h>
#include <bits/stdc++.h>
using namespace std;

#define KEY "zE20Ar15ckS"
//Key is used for encryption of passwords

int XSize,Order_number;
//XSize is the screen width in spaces, Order_number stores order number in a shop

void delay(clock_t delay_value){
    clock_t temp;
    temp=clock();
    while(clock()-temp<delay_value){}
}
//delay function can pause the process for delay_value milliseconds

void printtocenter(string s,int i){
    for(int ii=0;ii<(XSize-s.length())/2;ii++)cout<<" ";
    if(i==1)cout<<s<<endl;
    if(i==2)cout<<s;
}
//printtocenter prints string s to the center of the screen, i denotes which line to print in

class Credentials{
private:
    string username,password;
    vector<string> Usernames,Passwords;
    //These data members can not be kept public

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
            system("cls");
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
            c=_getch();
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
        return;
    }
    //All menu items and previous orders are output into respective files

    void clear_order(){
        system("cls");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Enter the token number to clear order",1);
        printtocenter("Press x to go back",1);
        printtocenter("",2);
        string j;
        char g;
        g=_getch();
        if(g=='x')return;
        while(g!=13){
            cout<<g;
            j.push_back(g);
            g=_getch();
        }
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
            system("cls");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Order not found!",2);
            delay(3000);
            this->clear_order();
            return;
        }
        this->Uninit();
        return;
    }
    // A particular order from the existing orders can be removed if token number is known.

    void existing_item(){
        system("cls");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        char c='\0';
        printtocenter("Do you want to edit a existing Menu item?(y/n) ",2);
        while(c!='n'&&c!='y')c=_getch();
        if(c=='n')return;
        system("cls");
        cout<<endl;
        printtocenter("*** "+Shop_name+" ***",1);
        printtocenter("Owner : "+Shop_owner,1);
        cout<<endl<<endl;
        this->Init();
        if(menu.size()==0){
            cout<<endl<<endl;
            printtocenter("*** No Items in Menu ***",1);
            delay(3000);
            return;
        }
        printtocenter("****  Menu  ****",1);
        cout<<endl<<setw(20)<<"S.No"<<setw(40)<<"Item Name"<<setw(50)<<"Quantity in Stock"<<setw(25)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<menu.size();ii++){
            cout<<setw(20)<<ii+1<<setw(40)<<menu[ii]->item_name<<setw(43)<<menu[ii]->stock<<setw(31)<<menu[ii]->cost<<endl<<endl;
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
            delay(2000);
            this->existing_item();
            return;
        }
        c='\0';
        printtocenter("Do you want to edit or delete item?(e/d) ",2);
        while(c!='e'&&c!='d')c=_getch();
        if(c=='d'){
            menu.erase(menu.begin()+k-1);
        }else{
            system("cls");
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
            while(c!='y'&&c!='n')c=_getch();
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
        system("cls");
        cout<<endl;
        printtocenter("*** "+Shop_name+" ***",1);
        printtocenter("Owner : "+Shop_owner,1);
        cout<<endl<<endl;
        this->Init();
        if(orders.size()==0){
            cout<<endl<<endl;
            printtocenter("*** No Orders Pending ***",1);
            delay(2000);
            return;
        }
        printtocenter("*** Pending Orders ***",1);
        cout<<endl<<setw(30)<<"Token No"<<setw(40)<<"Item Name"<<setw(30)<<"Quantity"<<setw(25)<<"Cost"<<endl<<endl<<endl;
        int ii;
        for(int ii=0;ii<orders.size();ii++){
            cout<<setw(28)<<orders[ii]->token_number;
            int co=0;
            for(int jj=0;jj<(orders[ii]->cost).size();jj++){
                cout<<setw((jj==0)?42:70)<<orders[ii]->name[jj]<<setw(27)<<orders[ii]->quantity[jj]<<setw(28)<<orders[ii]->cost[jj]<<endl;
                co+=orders[ii]->cost[jj];
            }
            cout<<setw(125)<<co<<endl<<endl<<endl;
        }
        printtocenter("Press r to refresh",1);
        printtocenter("Press x to go back",2);
        char c='\0';
        while(c!='x'&&c!='r')c=_getch();
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
        system("cls");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Do you want to add a new item?(y/n) ",2);
        char c='\0';
        while(c!='y'&&c!='n')c=_getch();
        if(c=='n')return;
        if(c=='y'){
            system("cls");
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
            while(d!='y'&&d!='n')d=_getch();
            if(d=='n'){
                this->new_item();
                return;
            }else{
                this->Init();
                for(int ii=0;ii<menu.size();ii++){
                    if(menu[ii]->item_name==s){
                        system("cls");
                        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
                        printtocenter("Item already present in the Menu!",2);
                        char e=_getch();
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
            system("cls");
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
            while(c!='1'&&c!='2'&&c!='3'&&c!='4'&&c!='5')c=_getch();
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
            system("cls");
            cout<<endl<<endl<<endl;
            this->pr_cart();                    //Selected items are printed
            cout<<endl<<endl<<endl;
            printtocenter("Press a to Add New Item",1);
            printtocenter("Press Enter to Finish Order",1);
            printtocenter("Press x to go back",1);
            printtocenter("",2);
            c='\0';
            while(c!='a'&&c!=13&&c!='x')c=_getch();
            if(c=='x'||c==13)break;
            sel.resize(menu.size());
            int q=this->sel_item('a');          //sel_item handles selection of an item from the menu
            if(q==-1)continue;
            system("cls");
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
            while(f!='n'&&f!='y')f=_getch();
            if(f=='n')continue;
            menu[q]->stock-=qua;
            temp=new Item(menu[q]->item_name,(menu[q]->cost)*qua,qua);
            cart.push_back(temp);
        }
        if(c=='x')return;
        if(cart.size()==0)return;
        system("cls");
        cout<<endl<<endl<<endl;
        printtocenter("Order placed Successfully",1);
        cout<<endl<<endl<<endl;
        printtocenter("Token Number : "+to_string(Order_number),1);
        cout<<endl<<endl<<endl;
        this->pr_cart();
        cout<<endl<<endl<<endl;
        printtocenter("Press x to go back ",2);
        char g='\0';
        while(g!='x')g=_getch();
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
        system("cls");
        cout<<endl<<endl;
        printtocenter("Select an Item",1);
        cout<<endl<<endl;
        for(int ii=0;ii<sel.size();ii++)sel[ii]=true;
        pr_items("");
        if(d!='e'&&d!='s'&&d!='x')printtocenter("Press e to enter index,s to search or x to go back(e/s/x) ",2);
        char c=d;
        while(c!='s'&&c!='e'&&c!='x')c=_getch();
        if(c=='x')return -1;
        if(c=='s'){
            cout<<endl<<endl<<endl;
            printtocenter("Enter Search key : ",2);
            string sea;
            cin>>sea;
            system("cls");
            cout<<endl<<endl;
            printtocenter("Select an Item",1);
            cout<<endl<<endl;
            for(int ii=0;ii<sel.size();ii++)sel[ii]=true;
            pr_items(sea);
            cout<<endl;
            printtocenter("Press e to enter index,s to search or x to go back(e/s/x) ",2);
            char e='\0';
            while(e!='x'&&e!='s'&&e!='e')e=_getch();
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
            delay(2000);
            return this->sel_item('a');
        }
        if(sel[p]==false){
            cout<<endl<<endl;
            printtocenter("Serial no. not among Search results!",2);
            delay(2000);
            return this->sel_item('a');
        }
        return p;
    }
    //Selection of an item from the menu is handled here.

    void pr_items(string s){
        if(s!=""){
            transform(s.begin(),s.end(),s.begin(),::tolower);
            for(int ii=0;ii<menu.size();ii++){
                if(this->isss(s,menu2[ii],s.size(),menu2[ii].size())==false)sel[ii]=false;
            }
        }
        int cou=0;
        for(int ii=0;ii<sel.size();ii++)if(sel[ii]&&menu[ii]->stock!=0)cou++;
        if(cou==0){
            printtocenter("No matches found!",1);
            return;
        }
        cout<<setw(40)<<"S.No"<<setw(40)<<"Item Name"<<setw(30)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<menu.size();ii++){
            if(sel[ii]==true&&menu[ii]->stock!=0){
                cout<<setw(40)<<ii+1<<setw(40)<<menu[ii]->item_name<<setw(29)<<menu[ii]->cost<<endl<<endl;
            }
        }
    }
    //All items in the menu containing subsequence s are printed

    void pr_cart(){
        if(cart.size()==0){
            printtocenter("Cart is Empty!",1);
            return;
        }
        printtocenter("****  Cart  ****",1);
        cout<<endl<<endl;
        cout<<setw(20)<<"S.No"<<setw(40)<<"Item Name"<<setw(50)<<"Quantity"<<setw(25)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<cart.size();ii++){
            cout<<setw(20)<<ii+1<<setw(40)<<cart[ii]->item_name<<setw(43)<<cart[ii]->stock<<setw(31)<<cart[ii]->cost<<endl<<endl;
        }
        cout<<endl;
    }
    //all items in the cart are printed

    void prev_orders(){
        system("cls");
        ifstream fin;
        fin.open("prev_order.txt");
        int k;
        fin>>k;
        this->refresh();
        cout<<endl<<endl;
        if(k==0){
            cout<<endl;
            printtocenter("No record of Previous Orders!",1);
            delay(2500);
            return;
        }
        printtocenter("*************************",1);
        printtocenter("***  Previous Orders  ***",1);
        printtocenter("*************************",1);
        cout<<endl<<endl;
        cout<<setw(20)<<"Shop Name"<<setw(30)<<"Token No"<<setw(40)<<"Item Name"<<setw(30)<<"Quantity"<<setw(25)<<"Cost"<<endl<<endl<<endl;
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
            cout<<setw(20)<<shop_names[b1]<<setw(27)<<a1<<setw(43)<<e1[0]<<setw(27)<<f1[0]<<setw(28)<<g1[0]<<endl;
            i1+=g1[0];
            for(int jj=1;jj<c1;jj++){
                cout<<setw(90)<<e1[jj]<<setw(27)<<f1[jj]<<setw(28)<<g1[jj]<<endl;
                i1+=g1[jj];
            }
            cout<<setw(145)<<i1<<endl<<endl<<endl;
        }
        fin.close();
        printtocenter("Press x to go back",2);
        char c='\0';
        while(c!='x')c=_getch();
        return;
    }
    //A list of all previous orders is printed. When a shop is deleted, the entire list is cleared.

    int shop_sel(char d){
        system("cls");
        cout<<endl<<endl;
        printtocenter("Select a Shop",1);
        cout<<endl<<endl;
        for(int ii=0;ii<sel.size();ii++)sel[ii]=true;
        pr_shops("");
        if(d!='e'&&d!='s'&&d!='x')printtocenter("Press e to enter index,s to search or x to go back(e/s/x) ",2);
        char c=d;
        while(c!='s'&&c!='e'&&c!='x')c=_getch();
        if(c=='x')return -1;
        if(c=='s'){
            cout<<endl<<endl<<endl;
            printtocenter("Enter Search key : ",2);
            string sea;
            cin>>sea;
            system("cls");
            cout<<endl<<endl;
            printtocenter("Select a Shop",1);
            cout<<endl<<endl;
            for(int ii=0;ii<sel.size();ii++)sel[ii]=true;
            pr_shops(sea);
            cout<<endl;
            printtocenter("Press e to enter index,s to search or x to go back(e/s/x) ",2);
            char e='\0';
            while(e!='x'&&e!='s'&&e!='e')e=_getch();
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
            delay(2000);
            return this->shop_sel('a');
        }
        if(sel[p]==false){
            cout<<endl<<endl;
            printtocenter("Serial no. not among Search results!",2);
            delay(2000);
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
                cout<<setw(77)<<ii<<"\t"<<shop_names[ii]<<endl<<endl;
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
        system("cls");
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
            delay(3000);
            return;
        }
        printtocenter("****  Menu  ****",1);
        cout<<endl<<setw(20)<<"S.No"<<setw(40)<<"Item Name"<<setw(50)<<"Quantity in Stock"<<setw(25)<<"Cost"<<endl<<endl<<endl;
        for(int ii=0;ii<menu.size();ii++){
            cout<<setw(20)<<ii+1<<setw(40)<<menu[ii]->item_name<<setw(43)<<menu[ii]->stock<<setw(31)<<menu[ii]->cost<<endl<<endl;
        }
        cout<<endl;
        printtocenter("Press x to go back ",2);
        char f='\0';
        while(f!='x')f=_getch();
        return;
    }
    //All menu items of a shop are printed

    void edit_shops(){
        system("cls");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Do you want to add or remove an existing shop?(a/r) ",1);
        printtocenter("Press x to go back",1);
        printtocenter("",2);
        char c='\0';
        while(c!='x'&&c!='r'&&c!='a')c=_getch();
        if(c=='x')return;
        else{
            system("cls");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";;
            printtocenter("Enter password to proceed : ",2);
            string pas;
            char c='\0';
            while(c!=13){
                c=_getch();
                pas.push_back(c);
            }
            pas.pop_back();
            Credentials D("master",pas);
            D.Init();
            int u=D.Validate();
            if(u!=0){
                cout<<endl<<endl;
                printtocenter("Wrong Password!",1);
                delay(2000);
                return;
            }
        }
        if(c=='r'){
            this->refresh();
            system("cls");
            cout<<"\n\n\n";
            if(shop_names.size()==1){
                printtocenter("No Existing shops to remove!",2);
                delay(3000);
                return;
            }
            printtocenter("***  Existing Shops  ***",1);
            cout<<"\n\n";
            for(int ii=1;ii<shop_names.size();ii++){
                cout<<setw(77)<<ii<<"\t"<<shop_names[ii]<<endl;
            }
            cout<<endl<<endl;
            printtocenter("Enter the Serial No. of the shop you want to remove : ",2);
            int k=0;
            cin>>k;
            if(k<=0||k>shop_names.size()){
                cout<<endl;
                printtocenter("Invalid Serial No!",1);
                delay(3000);
                return;
            }
            ofstream f;
            f.open("prev_order.txt");
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
            system("cls");
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
            while(c!='n'&&c!='y')c=_getch();
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
        HANDLE screen = GetStdHandle( STD_OUTPUT_HANDLE );
        COORD max_size = GetLargestConsoleWindowSize( screen );
        XSize=max_size.X;
        system("cls");
        string Welcome_string="Welcome to Canteen Management Portal...";
        delay(400);
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter(Welcome_string,0);
        for(int ii=0;Welcome_string[ii]!='\0';ii++){
            cout<<Welcome_string[ii];
            delay(50);
        }
        delay(400);
        for(int ii=0;ii<Welcome_string.length();ii++){
            cout<<"\b \b";
            delay(25);
        }
    }
    //Welcome message is printed

    void Start(){
        system("cls");
        char c;
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("1. Login",1);
        cout<<endl;
        printtocenter("2. Exit",1);
        cout<<endl;
        printtocenter("Enter appropriate digit : ",2);
        c=_getch();
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
        system("cls");
        string pw_stars;
        char c='\0';
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Enter Username : ",2);
        cin>>username;
        password.clear();
        while(c!=13){
            system("cls");
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Enter Password : ",2);
            cout<<pw_stars;
            pw_stars.push_back('*');
            c=_getch();
            password.push_back(c);
        }
        password.pop_back();
        system("cls");
        Credentials C(username,password);
        C.Init();
        int i=C.Validate();     //Username and Password are validated
        if(i==-1){
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Username not found!!!",1);
            delay(500);
            char c=_getch();
            this->Login();
            return;
        }                       //Username not found case
        if(i==-2){
            cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
            printtocenter("Wrong Password!!!",1);
            delay(500);
            char c=_getch();
            this->Login();
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
        system("cls");
        cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        printtocenter("Thank you",1);
        delay(1000);
        return;
    }
    //End prints thank you message

};
//This class handles the procedure that is common to both cashier and the shops.

int main(){
    int i;
    Order_number=1;
    Print P;
    P.Start();
    P.End();
    return 0;
}
