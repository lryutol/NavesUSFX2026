#pragma once

template<typename T>
class Lista
{
private:
    struct Nodo
    {
        T dato;
        Nodo* siguiente;
        Nodo(const T& d) : dato(d), siguiente(nullptr) {}
    };

    Nodo* cabeza;

public:
    Lista() : cabeza(nullptr) {}
    ~Lista() { vaciar(); }

    void insertar(const T& elemento)
    {
        Nodo* nuevo = new Nodo(elemento);
        if (!cabeza)
        {
            cabeza = nuevo;
        }
        else
        {
            Nodo* actual = cabeza;
            while (actual->siguiente)
                actual = actual->siguiente;
            actual->siguiente = nuevo;
        }
    }

    bool eliminar(const T& elemento)
    {
        Nodo* actual = cabeza;
        Nodo* anterior = nullptr;
        while (actual)
        {
            if (actual->dato == elemento)
            {
                if (anterior) anterior->siguiente = actual->siguiente;
                else cabeza = actual->siguiente;
                delete actual;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        return false;
    }

    bool vacia() const { return cabeza == nullptr; }

    void vaciar()
    {
        Nodo* actual = cabeza;
        while (actual)
        {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        cabeza = nullptr;
    }

    template<typename Func>
    void forEach(Func funcion) const
    {
        Nodo* actual = cabeza;
        while (actual)
        {
            funcion(actual->dato);
            actual = actual->siguiente;
        }
    }

    class iterator
    {
        Nodo* nodo;
    public:
        iterator(Nodo* n) : nodo(n) {}
        T& operator*() { return nodo->dato; }
        iterator& operator++() { nodo = nodo->siguiente; return *this; }
        bool operator!=(const iterator& otro) const { return nodo != otro.nodo; }
    };

    iterator begin() { return iterator(cabeza); }
    iterator end() { return iterator(nullptr); }
};