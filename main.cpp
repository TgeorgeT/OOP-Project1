#include <iostream>
#include <cstring>
#include <string>

using namespace std;

class Nod {
    int val;
    Nod *n, *p;
public:
    Nod(int v = 0) {
        val = v;
        n = p = nullptr;
    }

    ~Nod() {
        n = p = nullptr;
    }

    Nod *next() {
        return n;
    }

    Nod *prev() {
        return p;
    }

    void setNext(Nod *a) {
        n = a;
    }

    void setPrev(Nod *a) {
        p = a;
    }

    void setValue(int v) {
        val = v;
    }

    int getValue() {
        return val;
    }

    Nod operator=(Nod);

};

Nod Nod::operator=(Nod a) {
    n=a.n;
    p=a.p;
    val=a.val;
    return *this;
}

class Lista {
    Nod *start, *final;
    int n;


public:

    Lista() {
        start = final = nullptr;
        n = 0;
    }

    Lista(int v) {
        start = final = new Nod(v);
        n = 1;
    }

    Lista(const Lista &l) {
        if (l.n == 0) {
            final = start = NULL;
            n = 0;
            return;
        }
        final = start = new Nod(l.start->getValue());
        n = 1;
        for (Nod *aux = l.start->next(); aux != nullptr; aux = aux->next()) {
            this->adaugare(n, aux->getValue());
        }
    }

    ~Lista() {
        if (n == 0)
            return;
        Nod *aux = start->next();
        while (aux != nullptr) {
            delete (aux->prev());
            aux = aux->next();
        }
        delete final;
        final = start = nullptr;
        n = 0;
    }


    int interogarePoz(int poz) {
        if ((*this)[poz] == nullptr)
            return -1;
        //this->[poz] -> nu merge
        //this->operator[](poz) -> merge
        // Nod *a=(*this).[poz];  -> (*this).operator[](poz)
        return (*this)[poz]->getValue();
    }

    int getNr() {
        return n;
    }

    Nod *operator[](int poz);

    friend ostream &operator<<(ostream &, Lista);

    friend istream &operator>>(istream &, Lista &);

    Lista operator+(int);

    friend Lista operator+(int, Lista);

    Lista operator=(Lista);

    Lista operator+(Lista);

    void eliminare(int);

    void adaugare(int, int);

    void push_back(int);

    void push_front(int);

    int pop_front();

    int pop_back();
};

Nod *Lista::operator[](int poz) {

    if (poz > n)
        return nullptr;
    Nod *aux;
    for (aux = start; poz > 0 && aux != nullptr; poz--)
        aux = aux->next();
    return aux;
}

int Lista::pop_back() {
    if (n == 0)
        return -1;
    eliminare(n - 1);
    return 0;
}

int Lista::pop_front() {
    if (n == 0)
        return -1;
    eliminare(0);
    return 0;

}

void Lista::eliminare(int poz) {
    if (poz >= n || poz < 0)
        return;

    if (poz == 0) {
        Nod *aux = start;
        start = start->next();
        delete aux;
        n--;
        if (start == nullptr) {
            final = start;
            return;
        }
        start->setPrev(nullptr);
        return;
    }
    if (poz == n - 1) {
        Nod *aux = final;
        final = final->prev();
        delete aux;
        n--;
        if (final == nullptr) {
            start = final;
            return;
        }
        final->setNext(nullptr);
        return;
    }
    Nod *aux = (*this)[poz];
    aux->prev()->setNext(aux->next());
    aux->next()->setPrev(aux->prev());
    n--;
    delete aux;
}

void Lista::push_front(int val) {
    this->adaugare(0, val);
}


istream &operator>>(istream &in, Lista &a) {
    int val;
    in >> val;
    a.push_back(val);
}

Lista Lista::operator+(Lista l) {
    Lista sum;
    for (Nod *aux = this->start; aux != nullptr; aux = aux->next())
        sum.push_back(aux->getValue());
    for (Nod *aux = l.start; aux != nullptr; aux = aux->next())
        sum.push_back(aux->getValue());
    return sum;
}

void Lista::push_back(int val) {
    adaugare(n, val);
}

Lista Lista::operator=(Lista l) {
    this->~Lista();
    final = start = new Nod(l.start->getValue());
    n = 1;
    for (Nod *aux = l.start->next(); aux != nullptr; aux = aux->next()) {
        this->adaugare(n, aux->getValue());
    }
    return *this;
}

Lista Lista::operator+(int val) {
    Lista sum = *this;
    sum.adaugare(sum.n, val);
    return sum;
}

Lista operator+(int val, Lista a) {
    a.adaugare(0, val);
    return a;
}

ostream &operator<<(ostream &out, Lista a) {
    if (a.n == 0)
        return out;
    for (Nod *aux = a.start; aux != nullptr; aux = aux->next())
        out << aux->getValue() << " ";
    return out;
}

void Lista::adaugare(int poz, int val) {
    Nod *aux = new Nod(val);
    if (poz < 0 || poz > n)
        return;
    if (n == 0) {
        n = 1;
        start = final = aux;
        return;
    }
    if (poz == 0) {
        start->setPrev(aux);
        aux->setPrev(nullptr);
        aux->setNext(start);
        start = aux;
        n++;
        return;
    }
    if (poz == n) {
        final->setNext(aux);
        aux->setPrev(final);
        aux->setNext(nullptr);
        final = aux;
        n++;
        return;
    }
    Nod *nod = (*this)[poz];
    aux->setPrev(nod->prev());
    aux->prev()->setNext(aux);
    aux->setNext(nod);
    nod->setPrev(aux);
    n++;
}

class Dequeue {
    Lista d;
public:
    Dequeue(int val = 0) : d(val) {}

    Dequeue(const Dequeue &a) : d(a.d) {}


    ~Dequeue() {
        d.~Lista();
    }

    void push_back(int val) {
        d.push_back(val);
    }

    void push_front(int val) {
        d.push_front(val);
    }

    int pop_back() {
        return d.pop_back();
    }

    int pop_front() {
        return d.pop_front();
    }

    int front() {
        return d.interogarePoz(0);
    }

    int back() {
        return d.interogarePoz(d.getNr() - 1);
    }

    friend ostream &operator<<(ostream &, Dequeue);

    Dequeue operator=(Dequeue);

    Dequeue operator+(int);

    void operator+=(int val) {
        d = d + val;
    }

    friend Dequeue operator+(int, Dequeue);

    operator int() {
        return this->front();
    }

    Nod *operator[](int);

};

Nod *Dequeue::operator[](int poz) {
    return d[poz];

}

ostream &operator<<(ostream &out, Dequeue a) {
    cout << a.d;
    return out;
}

Dequeue operator+(int val, Dequeue a) {
    a.push_front(val);
    return a;
}

Dequeue Dequeue::operator=(Dequeue a) {
    this->~Dequeue();
    this->d = a.d;
    return *this;
}

Dequeue Dequeue::operator+(int val) {
    Dequeue sum = *this;
    sum.push_back(val);
    return sum;
}

inline void rtrim(string &s) {
    s.erase(s.find_last_not_of(' ') + 1);
}

inline void ltrim(string &s) {
    s.erase(0, s.find_first_not_of(' '));
}

inline void trim(string &s) {
    ltrim(s);
    rtrim(s);
}

int isnum(string a)
{
    for(int i=0;i<a.length();i++)
        if(!(a[i]>='0' && a[i]<='9'))
            return -1;
    return 0;
}

int main() {
    string ans;
    trim(ans);
    cout << "Do you want to create a new dequeue?(y/n)\n";
    while (1) {
        getline(cin, ans);
        trim(ans);
        if (ans.length() != 1 || strchr("ny",ans[0])==NULL)
            cout << "Please type (y/n)\n";
        else {
            if (tolower(ans[0]) == 'n')
                exit(0);
            break;
        }
    }
    cout << "#########################################\n\n";
    cout << "Insert the first number into the dequeue:\n";
    int nr;
    while (1) {
        string number;
        getline(cin, number);
        trim(number);
        try {
            if(isnum(number)==-1)
                throw invalid_argument("");
            nr = stoi(number);
        }
        catch (const exception &e) {
            cout << "Invalid number\n";
            continue;
        }

        break;
    }
    Dequeue d(nr);
    while (1) {
        cout << "\n#########################################\n\n";
        cout << "Your dequeue:\n" << d << "\n\n";
        cout << "Pick an option:\n";
        cout << "0. Exit\n";
        cout << "1. Push back a number\n";
        cout << "2. Push front a number\n";
        cout << "3. Pop back\n";
        cout << "4. Pop front\n";
        cout << "5. Get the front element\n";
        cout << "6. Get the back element\n";
        cout << "7. Get the element at the n-th position\n";
        getline(cin, ans);
        int option = -1;
        try {
            option = stoi(ans);
            if (option > 7)
                throw invalid_argument("");
        }
        catch (const exception &e) {
            cout << "Please type a number from 0 to 4\n";
            continue;
        }
        if (option == 0) {
            exit(1);
        }
        if (option == 1) {
            while (1) {

                cout << "\n#########################################\n\n";
                cout << "Type b to go back or type a number:\n";
                string number;
                int nr;
                getline(cin, number);
                trim(number);
                if (number == "b")
                    break;
                try {
                    if(isnum(number)==-1)
                        throw invalid_argument("");
                    nr = stoi(number);
                }
                catch (const exception &e) {
                    cout << "Invalid input\n";
                    continue;
                }
                d.push_back(nr);
                break;
            }
        } else if (option == 2) {
            while (1) {

                cout << "\n#########################################\n\n";
                cout << "Type b to go back or type a number:\n";
                string number;
                int nr;
                getline(cin, number);
                trim(number);
                if (number == "b")
                    break;
                try {
                    if(isnum(number)==-1)
                        throw invalid_argument("");
                    nr = stoi(number);
                }
                catch (const exception &e) {
                    cout << "Invalid input\n";
                    continue;
                }
                d.push_front(nr);
                break;
            }

        } else if (option == 3) {
            try {
                if (d.pop_back())
                    throw 1;
            }
            catch (int x) {
                cout << "\nDequeue is empty\n";
            }
        } else if (option == 4) {
            try {
                if (d.pop_front())
                    throw 1;
            }
            catch (int x) {
                cout << "\nDequeue is empty\n";
            }
        } else if (option == 5) {
            cout << (int) d << '\n';
        } else if (option == 6) {
            cout << d.back() << '\n';
        } else if (option == 7) {
            while (1) {
                cout << "\n#########################################\n\n";
                cout << "Type b to go back or type a position:\n";
                string number;
                int nr;
                getline(cin, number);
                trim(number);
                if (number == "b")
                    break;
                try {
                    if(isnum(number)==-1)
                        throw invalid_argument("");
                    nr = stoi(number);
                }
                catch (const exception &e) {
                    cout << "Invalid input\n";
                    continue;
                }
                Nod *aux;
                try {
                    aux = d[nr];
                    if (aux == nullptr)
                        throw 1;
                }
                catch (int val) {
                    cout << "Position out of range\n";
                    continue;
                }
                cout << aux->getValue()<<"\n";
                break;
            }
        }
    }
    return 0;
}
