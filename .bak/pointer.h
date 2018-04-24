#include <iostream>

#define GET_NAME(n) #n

using namespace std;

class Dummy
{
  public:
    bool isItMe(const Dummy &dummy);
    bool isItMe(Dummy *dummy) const;
    static void checkEqual(const Dummy &dummyReference, Dummy *dummyPointer, string dummyReferenceName, string dummyPointerName);
};

bool Dummy::isItMe(const Dummy &dummy)
{
    return (&dummy == this);
}

bool Dummy::isItMe(Dummy *dummy) const
{
    return (dummy == this);
}

void Dummy::checkEqual(const Dummy &dummyReference, Dummy *dummyPointer, string dummyReferenceName, string dummyPointerName)
{
    if (dummyPointer->isItMe(dummyReference))
    //if (&dummyReference == dummyPointer)
    {
        cout << "Yes, &" + dummyReferenceName + " is " + dummyPointerName + "" << endl;
    }
    else
    {
        cout << "No, &" + dummyReferenceName + " is not " + dummyPointerName + "" << endl;
    }

    if (dummyReference.isItMe(dummyPointer))
    //if (dummyPointer == &dummyReference)
    {
        cout << "Yes, *" + dummyPointerName + " is " + dummyReferenceName + "" << endl;
    }
    else
    {
        cout << "No, *" + dummyPointerName + " is not " + dummyReferenceName + "" << endl;
    }
}

int main()
{
    Dummy a;
    //Dummy a = *new Dummy();
    //Dummy &a = *new Dummy();
    Dummy *b = &a;
    Dummy &c = a;
    Dummy *e = &c;
    Dummy f = *b;
    Dummy &g = *b;

    Dummy::checkEqual(a, b, "a", "b");
    Dummy::checkEqual(c, b, "c", "b");
    Dummy::checkEqual(c, e, "c", "e");
    Dummy::checkEqual(f, b, "f", "b");
    Dummy::checkEqual(g, b, "g", "b");

    return 0;
}