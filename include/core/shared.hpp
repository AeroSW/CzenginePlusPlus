#include "inttypes.hpp"
#include <iostream>
#include <string>

template<typename T>
class Shared {
    public:
        Shared();
        Shared(T * ref);
        Shared(const Shared<T> &shared_ref);
        Shared(Shared<T> &&shared_ref);
        Shared(Shared<T> * shared_ref);
        ~Shared();

        T * get();

        Shared<T>& operator=(const Shared<T> &shared_ref);
        Shared<T>& operator=(Shared<T> &&shared_ref);
        bool operator!();
        const T * operator->();
        const T& operator*();
        const ui64 count();
    private:
        ui64 * counter;
        T * reference;
        void deref();
        void debug(const std::string &pre, const std::string &post);
};

// Shared<T>
template<typename T>
Shared<T>::Shared() {
    reference = nullptr;
    counter = new ui64(0);
    //debug("Empty", "Created.");
}
template<typename T>
Shared<T>::Shared(T * ref) {
    reference = ref;
    counter = new ui64(1);
    //debug("Nonempty", "Created.");
}

template<typename T>
Shared<T>::Shared(const Shared<T> &shared_ref) {
    counter = shared_ref.counter;
    (*counter)++;
    reference = shared_ref.reference;
    //debug("", "Reference Copied");
}

template<typename T>
Shared<T>::Shared(Shared<T> &&dying_ref) {
    counter = dying_ref.counter;
    reference = dying_ref.reference;
    dying_ref.reference = nullptr;
    dying_ref.counter = nullptr;
    //debug("", "Reference Moved");
}

template<typename T>
Shared<T>::Shared(Shared<T> * shared_ref) : counter(shared_ref->counter) {
    (*counter)++;
    reference = shared_ref->reference;
    //debug("", "Pointer Copy");
}

template<typename T>
Shared<T>::~Shared() {
    //debug("", "Destruction.");
    deref();
}

template<typename T>
T * Shared<T>::get() {
    //debug("", "Get Reference.");
    return reference;
}

template<typename T>
Shared<T>& Shared<T>::operator=(const Shared<T> &shared_ref) {
    deref();
    counter = shared_ref.counter;
    reference = shared_ref.reference;
    if (reference != nullptr) (*counter)++;
    //debug("", "assign reference.");
    return *this;
}

template<typename T>
Shared<T>& Shared<T>::operator=(Shared<T> &&dying_ref) {
    deref();
    counter = dying_ref.counter;
    reference = dying_ref.reference;
    dying_ref.counter = nullptr;
    dying_ref.reference = nullptr;
    //debug("", "assignment move reference.");
    return *this;
}

template<typename T>
bool Shared<T>::operator!() {
    //debug("", "bool operation.");
    return reference == nullptr;
}

template<typename T>
const T * Shared<T>::operator->() {
    //debug("", "point to reference's property.");
    return reference;
}

template<typename T>
const T& Shared<T>::operator*() {
    //debug("", "dereference.");
    return *reference;
}

template<typename T>
const ui64 Shared<T>::count() {
    //debug("", "get current count.");
    return ui64(counter/*->count*/);
}

template<typename T>
void Shared<T>::deref() {
    (*counter)--;
    if (*counter <= 0) {
        if(reference != nullptr) {
            delete reference;
        }
        delete counter;
    }
}
template<typename T>
void Shared<T>::debug(const std::string &pre, const std::string &post) {
    std::string active_count = (counter != nullptr) ? std::to_string(*counter) : "undefined";
    std::cout << " Ref: " << reference << " Count: " << active_count << " | " << pre << " Shared<T> " << post << "\n";
}
// end Shared<T>
