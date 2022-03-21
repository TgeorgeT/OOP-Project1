#include <iostream>

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

};

class Lista {
    Nod *start, *final;
    int n;

    Nod *operator[](int poz) {
        if (poz > n)
            return nullptr;
        Nod *aux;
        for (aux = start; poz > 0 && aux != nullptr; poz--)
            aux = aux->next();
        return aux;
    }

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

    void pop_front();

    void pop_back();
};

void Lista::pop_back() {
    eliminare(n - 1);
}

void Lista::pop_front() {
    eliminare(0);

}

void Lista::eliminare(int poz) {
    if (poz >= n || poz < 0)
        return;

    if (poz == 0) {
        Nod *aux = start;
        start = start->next();
        delete aux;
        start->setPrev(nullptr);
        n--;
        return;
    }
    if (poz == n - 1) {
        Nod *aux = final;
        final = final->prev();
        delete aux;
        final->setNext(nullptr);
        n--;
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

    void pop_back() {
        d.pop_back();
    }

    void pop_front() {
        d.pop_front();
    }

    int front() {
        return d.interogarePoz(0);
    }

    int back() {
        return d.interogarePoz(d.getNr());
    }

    friend ostream &operator<<(ostream &, Dequeue);

    Dequeue operator=(Dequeue);

    Dequeue operator+(int);

    friend Dequeue operator+(int, Dequeue);

    operator int() {
        return this->front();
    }

};

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

int main() {
    Dequeue a, d(2);
    a = d + 1;
    a = 1 + a;
    a.push_back(5);
    a.pop_front();
    a.push_front(10);
    int b=a;
    cout<<b;
    return 0;
}