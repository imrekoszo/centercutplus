#ifndef CLASSHELPERS_H
#define CLASSHELPERS_H

//
// disables copy constructor and assignment operator
//
// usage:
// class Foo
// {
//     // ...
// private:
//     DISALLOW_COPY_AND_ASSIGN(Foo);
//     // ...
// };
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&)

//
// disables default constructor
//
// usage:
// class Foo
// {
//     // ...
// private:
//     DISALLOW_INSTANTIATION(Foo);
//     // ...
// };
#define DISALLOW_INSTANTIATION(TypeName) \
    TypeName()

#endif // CLASSHELPERS_H
