#include <memory>

template <class T>
class node
{
using spNode = std::shared_ptr<node>;
public:
    node(const T&);
    node(spNode&);
    ~node();
    inline const T& getData() {return data;}
    inline void setData(T data) {this->data = data;}
    inline spNode& getOrigin() {return origin;}
    inline void setLeft(spNode n) {left = n;}
    inline spNode& getLeft() {return left;}
    inline void setRight(spNode n) {right = n;}
    inline spNode& getRight() {return right;}
private:
    T data;
    spNode origin = nullptr; // it just used when the node dup with another node
    spNode left = nullptr;
    spNode right = nullptr;
};

template <class T>
node<T>::node(const T& data)
    : data(data)
{
}

template <class T>
node<T>::node(spNode& origin)
    : data(origin->getData()), origin(origin)
{
    // never delete the origin
}

template <class T>
node<T>::~node()
{

}