#ifndef REFCOUNT_H
#define REFCOUNT_H

class RefCounter
{
public:
    RefCounter() : references(0) {}
    void addRef();
    bool deleteRef();
    int getReferences() const;

protected:
    int references;
};

#define SUPPORTS_REFCOUNTING \
    private: \
    RefCounter refCounter_; \
    public: \
    void addRef() { refCounter_.addRef(); } \
    bool deleteRef() { return refCounter_.deleteRef(); } \
    int getReferences() const { return refCounter_.getReferences(); } \
    RefCounter* refCounter() { return &refCounter_; }

class RefKeeper
{
public:
    RefKeeper(RefCounter* r) : rx(r) { r->addRef(); }
    inline ~RefKeeper() { rx->deleteRef(); }
protected:
    RefCounter* rx;
};

#endif // REFCOUNT_H

