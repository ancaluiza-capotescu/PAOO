/**
Item 4: Make sure that objects are initialized before they’re used.
Item 5: Know what functions C++ silently writes and calls.
Item 6: Explicitly disallow the use of compiler generated functions you do not want
*/

#include <bits/stdc++.h>
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

class Subject{ //Item 5 - va avea default constructor, destructor, copy constructor si copy assignment operator daca sunt cerute
};

class Classroom{
	public:
		Classroom(const string &name, const string &address, const int no_of_students);
		Classroom(const Classroom&);
		Classroom& operator=(const Classroom& u);
		~Classroom();
		string toString();
	
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
	this->no_of_students = u.no_of_students;
	return *this;
}
Classroom::~Classroom(){
	cout<<"Classroom destructor"<<endl;
}

string Classroom::toString(){
	return "Sala de clasa "+name+" se afla la adresa "+address+" si are capacitatea de "+to_string(no_of_students)+" studenti \n";
}

//Item 6 - nu are sens sa putem copia o universitate => disallow copy constructor and copy assignment operator
class University{
	public:
		University(const string &name, const string &address);
		~University();
		string toString();
	
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

int main()
{
	int a = 1;//Item 4 - always initialize objects
	cout << "a = "<<a<<endl;
	Student s("Anca");
	cout<< (s.toString()); 
	Subject subj1; //se apeleaza default constructorul deoarece nu l-am suprascris
	//la final se apeleaza si destructorul, inainte sa se iasa din main
	Subject subj2(subj1); //default copy constructor 
	subj2 = subj1; //default copy assignment operator
	Classroom c1("Laborator Programare","Vasile Parvan nr. 1",20); //constructor
	cout<< "c1 "<<(c1.toString());
	Classroom c2(c1); //copy constructor
	cout<< "c2 "<<(c2.toString());
	Classroom c3("Laborator PAOO","Vasile Parvan nr. 2",30);
	cout<< "c3 "<<(c3.toString());
	c3 = c1; //copy assignment operator
	cout<< "c3 "<<(c3.toString());
	University u1("UPT","Vasile Parvan nr. 1");
	//University u2(u1); - eroare de compilare
	
	return 0;
}
