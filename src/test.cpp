/**
Item 4(liniile 21, 31, 165): Make sure that objects are initialized before they’re used.
Item 5(liniile 40): Know what functions C++ silently writes and calls.
Item 6(liniile 137): Explicitly disallow the use of compiler generated functions you do not want
Item 10(liniile 80, 127): Have assignment operators return a reference to *this.
Item 11(liniile 73): Handle assignment to self in operator=.
Item 12(liniile 90, 111, 124, 190): Copy all parts of an object.

*/

#include <bits/stdc++.h>
#include <mutex>
using namespace std;

class Student{
	public:
		Student(const string &name);
		string toString();
	private:
		string name;
};
Student::Student(const string &name) //Item 4: initializare in constructor
:name(name)
{
	cout<<"Student constructor"<<endl;
}

string Student::toString(){
	return "Studentul se numeste "+name+"\n";
}

//Item 4 - daca avem mai multe translation units, trebuie sa ne asiguram ca nu e apelata metoda toString a clasei student fara a avea obiectul initializat
//deci creem asta si din alt translation unit(daca va fi cazul) va fi apelat stud().toString()
Student& stud()
{
	static Student stud("Stud");
	return stud;
}

//-----------------------------------------------------------------------------------------------------------------
class Subject{ //Item 5 - va avea default constructor, destructor, copy constructor si copy assignment operator daca sunt cerute
};

//-----------------------------------------------------------------------------------------------------------------
class Classroom{
	public:
		Classroom(const string &name, const string &address, const int no_of_students);
		Classroom(const Classroom&);
		Classroom& operator=(const Classroom& u);
		~Classroom();
		virtual string toString();
		
	
	private:
		string name;
		string address;
		int no_of_students;
};

Classroom::Classroom(const string &name, const string &address, const int no_of_students)
:name(name),
address(address),
no_of_students(no_of_students){
	cout<<"Classroom constructor"<<endl;
}
Classroom::Classroom(const Classroom& u){
	cout<<"Classroom copy constructor"<<endl;
	this->name = u.name;
	this->address = u.address;
	this->no_of_students = u.no_of_students;
}
Classroom& Classroom::operator=(const Classroom& u){
	cout<<"Classroom copy assignment operator"<<endl;
	if (this == &u){
		cout<<"Assign to itself - do nothing"<<endl; //Item 11,
	//other methods: careful statement ordering, copy and swap to avoid deleting a resource when you may still need it
		return *this;
	}
	this->name = u.name;
	this->address = u.address;
	this->no_of_students = u.no_of_students;
	return *this;  // Item 10: returned reference to this
}
Classroom::~Classroom(){
	cout<<"Classroom destructor"<<endl;
}

string Classroom::toString(){
	return "Sala de clasa "+name+" se afla la adresa "+address+" si are capacitatea de "+to_string(no_of_students)+" studenti \n";
}

//Item 12 - cream o clasa SmartClassroom ce e derivata din Classroom pentru a 
// evidentia importanta respectarii Item 12
class SmartClassroom: public Classroom{
	public:
		SmartClassroom(const string &name, const string &address, const int no_of_students, const bool smart_board, const int no_of_computers);
		SmartClassroom(const SmartClassroom&);
		SmartClassroom& operator=(const SmartClassroom& u);
		~SmartClassroom();
		virtual string toString();
	
	private:
		bool smart_board;
		int no_of_computers;	
};
SmartClassroom::SmartClassroom(const string &name, const string &address, const int no_of_students, const bool smart_board, const int no_of_computers)
:Classroom(name,address,no_of_students),
smart_board(smart_board),
no_of_computers(no_of_computers){
	cout<<"SmartClassroom constructor"<<endl;
}
SmartClassroom::SmartClassroom(const SmartClassroom& u)
: Classroom(u), //Item 12: nu uitam sa apelam copy constructorul clasei de baza pentru a copia si atributele mostenite din aceasta - name, address, no_of_students
smart_board(smart_board),
no_of_computers(no_of_computers)
{
	cout<<"SmartClassroom copy constructor"<<endl;
}
SmartClassroom& SmartClassroom::operator=(const SmartClassroom& u){
	cout<<"SmartClassroom copy assignment operator"<<endl;
	if (this == &u){
		cout<<"Assign to itself - do nothing"<<endl; //Item 11,
	//other methods: careful statement ordering, copy and swap to avoid deleting a resource when you may still need it
		return *this;
	}
	Classroom::operator=(u); //Item 12: apelam assignment copy op al clasei de baza pentru atributele mostenite din ea
	this->smart_board = u.smart_board;
	this->no_of_computers = u.no_of_computers;
	return *this;  // Item 10: returned reference to this
}
SmartClassroom::~SmartClassroom(){
	cout<<"SmartClassroom destructor"<<endl;
}
string SmartClassroom::toString(){
	return Classroom::toString() + " si are "+to_string(no_of_computers)+" calculatoare \n";
}


//Item 14 - implementam functionalitatea mutex pe ExamClassroom
class ExamClassroom: public Classroom{
	public:
		ExamClassroom(const string &name, const string &address, const int no_of_students, const bool isFree);
		~ExamClassroom();
		virtual string toString();
		void setIsFree(const bool value);
	
	private:
		bool isFree;
};
ExamClassroom::ExamClassroom(const string &name, const string &address, const int no_of_students, const bool isFree)
:Classroom(name,address,no_of_students),
isFree(isFree){
	cout<<"ExamClassroom constructor"<<endl;
}
string ExamClassroom::toString(){
	return Classroom::toString() + " si este libera(1) sau ocupata(0): "+to_string(isFree) + "\n";
}
void ExamClassroom::setIsFree(const bool value){
	isFree = value;
}
ExamClassroom::~ExamClassroom(){
	cout<<"ExamClassroom destructor"<<endl;
}
//Item 14 - folosirea de mutex

//LockClassroom
class LockClassroom{
	public:
		LockClassroom(ExamClassroom& examClassroom);
		~LockClassroom();
};
void lock(ExamClassroom &examClassroom)
{
    examClassroom.setIsFree(false);
    std::cout<<"ExamClassroom is locked. Someone is in it..."<<std::endl;
}

void unlock(ExamClassroom &examClassroom)
{
    examClassroom.setIsFree(true);
    std::cout<<"The examClassroom is now free!"<<std::endl;
}

LockClassroom::LockClassroom(ExamClassroom &examClassroom) : lockedClassroom_(examClassroom)
{
    lock(lockedClassroom_);
}

LockClassroom::~LockClassroom()
{
    unlock(lockedClassroom_);
}

void waitUntilAvailable(ExamClassroom *examClassroom)
{
    LockClassRoom *examClassroomLock = new LockClassRoom(*examClassroom);
    std::cout<<"ExamClassroom is free";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    delete examClassroomLock;
}




//------------------------------------------------------------------------------------------------------------------------
//Item 6 - nu are sens sa putem copia o universitate => disallow copy constructor and copy assignment operator
class University{
	public:
		University(const string &name, const string &address);
		~University();
		string toString();
		University* createUniversity();
	
	private:
		string name;
		string address;
		University(const University&);
		University& operator=(const University& u);
};
University::University(const string &name, const string &address)
:name(name),
address(address){
	cout<<"University constructor"<<endl;
}
University::~University(){
	cout<<"University destructor"<<endl;
}

string University::toString(){
	return "Universitatea "+name+" se afla la adresa "+address+"\n";
}

University* createUniversity(const string &name, const string &address){
	return new University(name,address);
}

int main()
{
	int a = 1;//Item 4 - always initialize objects
	cout << "a = "<<a<<endl;
	Student s("Anca");
	cout<< (s.toString()); 
	
	Subject subj1; //se apeleaza default constructorul deoarece nu l-am suprascris
	//la final se apeleaza si destructorul, inainte sa se iasa din main
	Subject subj2(subj1); //default copy constructor - se genereaza doar daca e nevoie de el
	subj2 = subj1; //default copy assignment operator- se genereaza doar daca e nevoie de el
	
	Classroom c1("Laborator Programare","Vasile Parvan nr. 1",20); //constructor
	cout<< "c1 "<<(c1.toString());
	Classroom c2(c1); //copy constructor
	cout<< "c2 "<<(c2.toString());
	Classroom c3("Laborator PAOO","Vasile Parvan nr. 2",30);
	cout<< "c3 "<<(c3.toString());
	c3 = c1; //copy assignment operator
	cout<< "c3 "<<(c3.toString());
	Classroom c4("Laborator FIET","Vasile Parvan nr. 3",20);
	cout<< "c4 "<<(c4.toString());
	c4 = c1; //copy assignment operator
	cout<< "c4 "<<(c4.toString());
	c1 = c1;
	cout<< "c1 "<<(c1.toString());
	
	//Item 12 - verificam daca s a facut bine clasa derivata 
	SmartClassroom sc1("Laborator Programare","Vasile Parvan nr. 1",20,true,10);
	cout<< "sc1 "<<(sc1.toString());
	SmartClassroom sc2 = sc1; //copy assignment operator
	cout<< "sc2 "<<(sc2.toString());
	SmartClassroom sc3(sc1); //copy constructor
	cout<< "sc3 "<<(sc3.toString());
	
	University u1("UPT","Vasile Parvan nr. 1");
	//University u2(u1); - eroare de compilare
	
		
	//Item 13
	//folosind smart pointeri, ne asiguram ca nu exista memory leaks
	auto_ptr<University> Univ1(createUniversity("UPT","Vasile Parvan"));
	cout<< "Univ1 "<<(Univ1->toString());
	auto_ptr<University> Univ2(Univ1);
	//cout<< "Univ1 "<<(Univ1->toString()); -> segmentation fault, pentru ca Univ1 a devenit null datorita liniei anterioare
	cout<< "Univ2 "<<(Univ2->toString());
	//OBSERVATIE: nu se va chema destructorul si pentru Univ1 deoarece deja a fost pus pe null
	
	//O universitate este unica, deci vom folosi unique_ptr pentru a crea unary_function
	unique_ptr<University> Univ3(createUniversity("vest","Vasile Parvan"));
	cout<< "Univ3 "<<(Univ3->toString());
	//unique_ptr<University> Univ4(Univ3); => eroare: nu putem copia un unique_ptr, acesta trebuind sa fie unic
	//dar putem folosi move:
	unique_ptr<University> Univ4 = move(Univ3);
	cout<< "Univ4 "<<(Univ4->toString());
	
	//Item 14 - folosim mutex pentru a verifica daca o sala de clasa e libera
	//Pentru a nu strica temele anterioare- vom crea o clasa derivata a clasei Classroom numita ExamClassroom care va avea un atribut boolean isFree
	ExamClassroom *occupied_examC = new ExamClassroom("Laborator Programare","Vasile Parvan nr. 1",20,false);
	cout<< "occupied_examC "<<(occupied_examC->toString());
	waitUntilAvailable(occupied_examC);
	cout<< "occupied_examC "<<(occupied_examC->toString());
	
	
	return 0;
}
