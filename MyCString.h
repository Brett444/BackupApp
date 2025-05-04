#pragma once

#include <string>
#include <QString>
#include "PndDefs.h"

class CColorRef
{
public:
	unsigned Red, Green, Blue;

    CColorRef(uint32 red, uint32 green, uint32 blue);
	CColorRef();
	~CColorRef();

    void SetColor(uint32 red, uint32 green, uint32 blue);

    bool operator==(const CColorRef& rhs) const;
};

//    Call a function and check ErrorFlag when the function returns.
//    Errors are reset inside the function at the start.
//    Errors are checked for immediately after the function returns. 

class CErrorInfo
{
public:
    std::string ErrorMsg;
    uint32 ErrorId;
    uint64 ErrorCnt;
	bool ErrorFlag;

	CErrorInfo();
	~CErrorInfo();

	void Reset();

    void Error(c_charptr errormsg, uint32 errorid=UINT32_MAX);
    void Error(std::string errormsg, uint32 errorid=UINT32_MAX);
    std::string ToString() const;
};

class CMyCString : public std::string
{
public:
    uint32 SType, ItemNum, RefCnt;
    bool SEnabled;

    CMyCString();
    CMyCString(const CMyCString &cref);
    CMyCString(c_charptr p_str);
    CMyCString(std::string stdstr);
    CMyCString(QString qstr);
    virtual ~CMyCString();

    uint32 GetLength() const;
    char GetAt(uint32 ind) const;
    bool IsEmpty() const;
    void Empty();
    char* CharArray() const;

    void InsertChar(uint32 idx, const char appchar);
    void InsertStr(uint32 idx, c_charptr appstr);
    void InsertStr(uint32 idx, CMyCString &appstr);

    void AppendChar(const char appchar);
    void AppendStr(c_charptr appstr);
    void AppendStr(const CMyCString &appstr);

    void PrependChar(const char appchar);
    void PrependStr(c_charptr appstr);
    void PrependStr(const CMyCString &appstr);

    static bool IsLegalVarChar(char c, bool isfirstchar);
    bool IsLegalVarName() const;
	void ToLower();
	void ToUpper();
    int32 CompareNoCase(c_charptr p_comparestr) const;
    int32 CompareNoCase(const CMyCString &comparestr) const;

    bool Contains(c_charptr substr, bool casesens=true, uint32 startpos=0) const;
    bool Contains(const CMyCString &substr, bool casesens=true, uint32 startpos=0) const;
    bool IsSubStrAt(CMyCString checkstr, bool casesens=true, uint32 checkloc=0) const;

    uint32 FindSubStr(c_charptr mstr, bool casesens=false, uint32 startpos=0) const;

    uint32 FindFree(const char tar, uint32 num=1) const;
    uint32 FindMatching(uint32 openind) const;
    CMyCString GetSubStrFree(uint32 num, char delimiter) const;

    int32 ToInt(boolptr p_convertok=nullptr) const;
    bool ToBool();
    flt64 ToDouble(boolptr p_convertok=nullptr) const;
    void FromColorRef(CColorRef color);
    CColorRef ToColorRef() const;
    QString ToQString() const;
    void FromQString(const QString &qstr);

	void StripChar(char ch);
	void StripComment();
    uint32 StripWhiteSpace(bool incommentalso = true);
    void StripLeadingWhiteSpace();
    void StripTrailingWhiteSpace();
    void StripLeft(uint32 count=1);
    void StripRight(uint32 count=1);
    void StripMid(uint32 start, uint32 count=1);
    void StripNameFromPath();
    bool StripTrailingNewlines();

    void GetComment(CMyCString& comment) const;
    CMyCString GetNameFromPath() const;
    CMyCString GetFileExtension() const;

    CMyCString GetLine(uint32& curidx) const;
    CMyCString GetSubStr1(uint32 start, uint32 stop) const;
    CMyCString GetSubStr2(int32 num, char delimiter, bool stripwhite=false) const;
    CMyCString GetSubStr3(int32 num, char delimiter1, char delimiter2, uint32ref stopcause) const;

    void Split(std::vector<std::string> &substrs, const std::string delims) const;
    void Join(const std::vector<std::string> &substrs, char delim);

    bool SaveString(const CMyCString &filepath, bool append) const;
    bool SaveString(c_charptr filepath, bool append) const;

    void Format(c_charptr formatstr, ...);

    CMyCString& operator=(c_charptr str);
    CMyCString& operator=(c_stringref rhs);
    CMyCString& operator=(const QString &rhs);
    CMyCString& operator=(const CMyCString &rhs);
};
typedef CMyCString* CMyCStringPtr;
typedef const CMyCString* CCMyCStringPtr;
typedef CMyCString& CMyCStringRef;
typedef const CMyCString& CCMyCStringRef;

/////////////////////////////////////////////////////////////////////////////////////////////

class CMyCStringList
{
    uint32 Count, Capacity;
    CMyCStringPtr* List;

	void IncreaseCapacity();

public:
    bool Modified;

    CMyCStringList(uint32 capacity=5);
    ~CMyCStringList();

    void Add(CMyCStringPtr pstring);
    bool InsertAt(CMyCStringPtr pstring, uint32 ind);
    bool RemoveAt(uint32 ind);
	void RemoveAll();
    bool ReplaceAt(CMyCStringPtr pstring, uint32 ind);
    CMyCStringPtr GetAt(uint32 ind) const;
    uint32 GetCount() const;
    uint32 GetCapacity() const;
    bool SetCapacity(uint32 capacity);
    void SetEqualTo(const CMyCStringList& stringlist);
    void Swap(uint32 i1, uint32 i2);
    CMyCString ToSingleStr(bool addcrs=false, bool adddblslashes=false) const;
    void FromSingleStr(CCMyCStringRef singlestr, bool stripleadingwhite=false, bool setenables=false);

    // CMyCStringPtr& operator[](uint32 ind) const;
};
typedef CMyCStringList* CMyCStringListPtr;
typedef const CMyCStringList* CCMyCStringListPtr;
typedef CMyCStringList& CMyCStringListRef;
typedef const CMyCStringList& CCMyCStringListRef;


template<typename Typ>
class CTList
{
    uint32 Count, Capacity;
    Typ** List;

    void IncreaseCapacity();

public:
    uint32 UseCnt;
    bool Modified, DelOnRemove;

    CTList(uint32 capacity=5);
    virtual ~CTList();

    void Add(Typ *pitem);
    bool InsertAt(Typ *pitem, uint32 ind);
    bool RemoveAt(uint32 ind);
    void RemoveAll();
    bool ReplaceAt(Typ *pitem, uint32 ind);
    Typ* GetAt(uint32 ind) const;
    Typ* GetLast() const;
    uint32 GetCount() const;
    uint32 GetCapacity() const;
    bool SetCapacity(uint32 capacity);
    void SetEqualTo(CTList<Typ> *pitem);
    void Swap(uint32 i1, uint32 i2);
};

template<typename Typ>
class CTListVal
{
    uint32 Count, Capacity;
    Typ* List;

    void IncreaseCapacity();

public:
    bool Modified;

    CTListVal(uint32 capacity=5);
    virtual ~CTListVal();

    void Add(Typ item);
    bool InsertAt(Typ item, uint32 ind);
    bool RemoveAt(uint32 ind);
    void RemoveAll();
    bool ReplaceAt(Typ item, uint32 ind);
    Typ GetAt(uint32 ind) const;
    Typ GetLast() const;
    uint32 GetCount() const;
    uint32 GetCapacity() const;
    bool SetCapacity(uint32 capacity);
    void SetEqualTo(CTListVal<Typ> *pitemlist);
    void Swap(uint32 i1, uint32 i2);
};

// FirstLoc means loc of first-in
// LastLoc means loc of last-in
template<typename Typ>
class CTCircList
{
    uint32 Count, Capacity, FirstLoc, LastLoc;
    Typ** List;

    void IncreaseCapacity();

public:
    bool Modified;
    bool DelOnRemove;

    CTCircList(uint32 capacity=20);
    virtual ~CTCircList();

    bool SetCapacity(uint32 capacity);
    bool Add(Typ *pitem);
    Typ* GetAt(uint32 ind) const;
    Typ* GetFirst(bool remove=true);
    Typ* GetLast() const;
    Typ* GetFromRaw(uint32 rawind) const;
    void RemoveAll();
    void RemoveFirst();
    uint32 GetCount() const;
    uint32 GetCapacity() const;
    bool IsFull() const;
    uint32 GetFirstLoc() const;
    uint32 GetLastLoc() const;
};

// FirstLoc means loc of first-in
// LastLoc means loc of last-in
template<typename Typ>
class CTCircListVal
{
    Typ* p_CircList;
    uint32 FirstLoc, LastLoc, Count;
    uint32 Capacity;

public:
    CTCircListVal();
    ~CTCircListVal();

    void SetCapacity(uint32 capacity);
    void RemoveAll();
    void RemoveFirst();
    bool AddEntry(Typ *p_entry);
    Typ* GetNextPtr(bool incptr=true);
    Typ* GetFirst(bool remove=true);
    Typ* GetLast() const;
    Typ* GetAt(uint32 ind) const;
    bool IsFull() const;
    uint32 GetCount() const;
};

///// For templates, the definitions need to be in the h file ///////////////

template<typename Typ>
CTList<Typ>::CTList(uint32 capacity)
{
    Modified = false;
	Count = 0;
    UseCnt = 0;
	Capacity = capacity;
	if (Capacity < 1) Capacity = 1;
	DelOnRemove = true;

    List = new Typ*[Capacity];
    for(uint32 i=0; i<Capacity; i++) List[i] = nullptr;
}

template<typename Typ>
CTList<Typ>::~CTList()
{
	RemoveAll();
	delete [] List;
}

template<typename Typ>
void CTList<Typ>::IncreaseCapacity()
{
    Typ** newlist;

	Capacity *= 2;
    newlist = new Typ*[Capacity];
    for(uint32 i=0; i<Count; i++) newlist[i] = List[i];
	delete [] List;
	List = newlist;
    for(uint32 i=Count; i<Capacity; i++) List[i] = nullptr;
}

template<typename Typ>
void CTList<Typ>::Add(Typ *pitem)
{
	if (Count == Capacity) IncreaseCapacity();
	List[Count] = pitem;
	Count++;
    Modified = true;
}

template<typename Typ>
bool CTList<Typ>::InsertAt(Typ *pitem, uint32 ind)
{
    assert(Count > 0 && "In CTList<Typ>::InsertAt");
    assert(ind < Count && "In CTList<Typ>::InsertAt");

    if (ind > Count) return(false);

	if (Count == Capacity)
		IncreaseCapacity();

    for(uint32 i=Count-1; i>=ind; i--)
		List[i+1] = List[i];

	List[ind] = pitem;
	Count++;
    Modified = true;
	return(true);
}

template<typename Typ>
bool CTList<Typ>::RemoveAt(uint32 ind)
{
    assert(Count > 0 && "In CTList<Typ>::RemoveAt");
    assert(ind < Count && "In CTList<Typ>::RemoveAt");

    if (ind >= Count) return(false);

    if (DelOnRemove)
    {
        if (List[ind] != nullptr)
            delete List[ind];
        List[ind] = nullptr;
    }

    for(uint32 i1=ind; i1<Count-1; i1++)
        List[i1] = List[i1+1];

    Count--;
    Modified = true;
    return(true);
}

template<typename Typ>
void CTList<Typ>::RemoveAll()
{
    if (Count > 0) Modified = true;

	if (DelOnRemove)
        for (uint32 i=0; i < Count; i++)
            delete List[i];

	Count=0;
    for(uint32 i=0; i<Capacity; i++) List[i] = nullptr;
}

template<typename Typ>
bool CTList<Typ>::ReplaceAt(Typ *pitem, uint32 ind)
{
    assert(Count > 0 && "In CTList<Typ>::ReplaceAt");
    assert(ind < Count && "In CTList<Typ>::ReplaceAt");

    if (ind >= Count) return(false);

	if (DelOnRemove)
		delete List[ind];

    List[ind] = pitem;
    Modified = true;
	return(true);
}

template<typename Typ>
Typ* CTList<Typ>::GetAt(uint32 ind) const
{
    assert(Count > 0 && "In CTList<Typ>::GetAt");
    assert(ind < Count && "In CTList<Typ>::GetAt");

    if (Count == 0) return(nullptr);

	if (ind >= Count) ind = Count-1;

	return(List[ind]);
}

template<typename Typ>
Typ* CTList<Typ>::GetLast() const
{
	if (Count == 0)
        return(nullptr);

	return(List[Count-1]);
}

template<typename Typ>
uint32 CTList<Typ>::GetCount() const
{
	return(Count);
}

template<typename Typ>
uint32 CTList<Typ>::GetCapacity() const
{
	return(Capacity);
}

template<typename Typ>
bool CTList<Typ>::SetCapacity(uint32 capacity)
{
    uint32 newcnt;
    Typ** newlist;

	if (capacity < 1) return(false);

    newlist = new Typ*[capacity];
    for(uint32 i=0; i<capacity; i++) newlist[i] = nullptr;

	if (Count <= capacity)
		newcnt = Count;
	else
		newcnt = capacity;
    for(uint32 i=0; i<newcnt; i++) newlist[i] = List[i];
    if (DelOnRemove) for(uint32 i=newcnt; i<Count; i++) delete(List[i]);
	delete [] List;
	List = newlist;
	Count = newcnt;
	Capacity = capacity;
	return(true);
}

template<typename Typ>
void CTList<Typ>::SetEqualTo(CTList<Typ> *pitemlist)
{
    Typ *p_item;

	RemoveAll();
    for(uint32 i=0; i<pitemlist->GetCount(); i++)
	{
        p_item = new Typ();
        *p_item = *(pitemlist->GetAt(i));
        Add(p_item);
	}

	Modified = pitemlist->Modified;
}

template<typename Typ>
void CTList<Typ>::Swap(uint32 i1, uint32 i2)
{
    Typ *pbuf;

	pbuf = List[i1];
	List[i1] = List[i2];
	List[i2] = pbuf;
}

// /////////////////////////////////////////////////////////////

template<typename Typ>
CTListVal<Typ>::CTListVal(uint32 capacity)
{
    Modified = false;
    Count = 0;
    Capacity = capacity;
    if (Capacity < 1) Capacity = 1;

    List = new Typ[Capacity];
}

template<typename Typ>
CTListVal<Typ>::~CTListVal()
{
    RemoveAll();
    delete [] List;
}

template<typename Typ>
void CTListVal<Typ>::IncreaseCapacity()
{
    Typ* newlist;

    Capacity *= 2;
    newlist = new Typ[Capacity];
    for(uint32 i=0; i<Count; i++) newlist[i] = List[i];
    delete [] List;
    List = newlist;
}

template<typename Typ>
void CTListVal<Typ>::Add(Typ item)
{
    if (Count == Capacity) IncreaseCapacity();
    List[Count] = item;
    Count++;
    Modified = true;
}

template<typename Typ>
bool CTListVal<Typ>::InsertAt(Typ item, uint32 ind)
{
    assert(ind <= Count && "In CTListVal<Typ>::GetAt");

    if (Count == 0)
    {
        Add(item);
        return(true);
    }

    if (Count == Capacity)
        IncreaseCapacity();

    for(uint32 i=Count-1; i>=ind; i--)
        List[i+1] = List[i];

    List[ind] = item;
    Count++;
    Modified = true;
    return(true);
}

template<typename Typ>
bool CTListVal<Typ>::RemoveAt(uint32 ind)
{
    assert(Count > 0 && "In CTListVal<Typ>::RemoveAt");
    assert(ind < Count && "In CTListVal<Typ>::RemoveAt");

    if (ind >= Count) return(false);

    for(uint32 i=ind; i<Count-1; i++)
        List[i] = List[i+1];
    Count--;
    Modified = true;
    return(true);
}

template<typename Typ>
void CTListVal<Typ>::RemoveAll()
{
    if (Count > 0) Modified = true;

    Count=0;
}

template<typename Typ>
bool CTListVal<Typ>::ReplaceAt(Typ item, uint32 ind)
{
    assert(Count > 0 && "In CTListVal<Typ>::ReplaceAt");
    assert(ind < Count && "In CTListVal<Typ>::ReplaceAt");

    if (ind >= Count) return(false);

    List[ind] = item;
    Modified = true;
    return(true);
}

template<typename Typ>
Typ CTListVal<Typ>::GetAt(uint32 ind) const
{
    Typ dflt;

    assert(Count > 0 && "In CTListVal<Typ>::GetAt");
    assert(ind < Count && "In CTListVal<Typ>::GetAt");

    if (Count == 0) return(dflt);

    if (ind >= Count) ind = Count-1;

    return(List[ind]);
}

template<typename Typ>
Typ CTListVal<Typ>::GetLast() const
{
    Typ dflt;

    if (Count == 0) return(dflt);

    return(List[Count-1]);
}

template<typename Typ>
uint32 CTListVal<Typ>::GetCount() const
{
    return(Count);
}

template<typename Typ>
uint32 CTListVal<Typ>::GetCapacity() const
{
    return(Capacity);
}

template<typename Typ>
bool CTListVal<Typ>::SetCapacity(uint32 capacity)
{
    uint32 newcnt;
    Typ* newlist;

    if (capacity < 1) return(false);

    newlist = new Typ[capacity];
    if (Count <= capacity)
        newcnt = Count;
    else
        newcnt = capacity;
    for(uint32 i=0; i<newcnt; i++) newlist[i] = List[i];
    delete [] List;
    List = newlist;
    Count = newcnt;
    Capacity = capacity;
    return(true);
}

template<typename Typ>
void CTListVal<Typ>::SetEqualTo(CTListVal<Typ> *pitemlist)
{
    RemoveAll();
    for(uint32 i=0; i<pitemlist->GetCount(); i++)
        Add(pitemlist->GetAt(i));

    Modified = pitemlist->Modified;
}

template<typename Typ>
void CTListVal<Typ>::Swap(uint32 i1, uint32 i2)
{
    Typ buf;

    buf = List[i1];
    List[i1] = List[i2];
    List[i2] = buf;
}

// /////////////////////////////////////////////////////////////

template<typename Typ>
CTCircList<Typ>::CTCircList(uint32 capacity)
{
    Modified = false;
    FirstLoc = LastLoc = Count = 0;
    Capacity = capacity;
    if (Capacity < 1) Capacity = 1;
    DelOnRemove = true;

    List = new Typ*[Capacity];

    for(uint32 i=0; i<Capacity; i++) List[i] = nullptr;
}

template<typename Typ>
CTCircList<Typ>::~CTCircList()
{
    RemoveAll();
    delete [] List;
}

template<typename Typ>
bool CTCircList<Typ>::SetCapacity(uint32 capacity)
{
    if (capacity < 1) return(false);

    RemoveAll();
    delete [] List;

    Capacity = capacity;
    List = new Typ*[Capacity];
    FirstLoc = LastLoc = Count = 0;
    for(uint32 i=0; i<Capacity; i++) List[i] = nullptr;

    return(true);
}

template<typename Typ>
bool CTCircList<Typ>::Add(Typ *pitem)
{
    if (IsFull()) return(false);

    Modified = true;
    if (Count) LastLoc += 1;
    Count += 1;
    if (LastLoc >= Capacity) LastLoc = 0;

    List[LastLoc] = pitem;

    return(true);
}

template<typename Typ>
void CTCircList<Typ>::RemoveAll()
{
    uint32 i, loc;

    if (Count > 0) Modified = true;

    loc = FirstLoc;
    if (DelOnRemove)
        for (i=0; i < Count; i++)
        {
            delete List[loc];
            loc += 1;
            if (loc >= Capacity) loc = 0;
        }

    FirstLoc = LastLoc = Count = 0;
    for(uint32 i=0; i<Capacity; i++) List[i] = nullptr;
}

template<typename Typ>
void CTCircList<Typ>::RemoveFirst()
{
    if (Count == 0) return;

    if (DelOnRemove)
    {
        delete List[FirstLoc];
        List[FirstLoc] = nullptr;
    }

    Count -= 1;
    if (Count > 0)
    {
        FirstLoc += 1;
        if (FirstLoc >= Capacity) FirstLoc = 0;
    }
}

template<typename Typ>
Typ* CTCircList<Typ>::GetAt(uint32 ind) const
{
    uint32 rawind;

    assert(Count > 0 && "In CTCircList<Typ>::GetAt");
    assert(ind < Count && "In CTCircList<Typ>::GetAt");

    if (Count == 0) return(nullptr);

    if (ind >= Count) ind = Count-1;

    rawind = FirstLoc + ind;
    if (rawind >= Capacity) rawind -= Capacity;

    return(List[rawind]);
}

template<typename Typ>
Typ* CTCircList<Typ>::GetFirst(bool remove)
{
    Typ* retptr;

    if (Count == 0) return(nullptr);

    retptr = List[FirstLoc];

    if (remove)
    {
        Count -= 1;

        if (Count > 0)
        {
            FirstLoc += 1;
            if (FirstLoc >= Capacity) FirstLoc = 0;
        }
    }

    return(retptr);
}

template<typename Typ>
Typ* CTCircList<Typ>::GetLast() const
{
    if (Count == 0) return(nullptr);

    return(List[LastLoc]);
}

template<typename Typ>
Typ* CTCircList<Typ>::GetFromRaw(uint32 rawind) const
{
    if (Count == 0) return(nullptr);
    if (rawind >= Capacity) return(nullptr);

    return(List[rawind]);
}

template<typename Typ>
uint32 CTCircList<Typ>::GetCount() const
{
    return(Count);
}

template<typename Typ>
uint32 CTCircList<Typ>::GetCapacity() const
{
    return(Capacity);
}

template<typename Typ>
bool CTCircList<Typ>::IsFull() const
{
    if (Count >= Capacity) return(true);

    return(false);
}

template<typename Typ>
uint32 CTCircList<Typ>::GetFirstLoc() const
{
    return(FirstLoc);
}

template<typename Typ>
uint32 CTCircList<Typ>::GetLastLoc() const
{
    return(LastLoc);
}

// ////////////////////////////////////////////////////////////

template<typename Typ>
CTCircListVal<Typ>::CTCircListVal()
{
    p_CircList = nullptr;
    FirstLoc = LastLoc = Count = 0;
    Capacity = 0;
}

template<typename Typ>
CTCircListVal<Typ>::~CTCircListVal()
{
    if (p_CircList != nullptr)
        delete [] p_CircList;
}

template<typename Typ>
void CTCircListVal<Typ>::SetCapacity(uint32 capacity)
{
    if (p_CircList != nullptr)
        delete [] p_CircList;

    FirstLoc = LastLoc = Count = 0;
    Capacity = capacity;

    p_CircList = new Typ[Capacity];
}

template<typename Typ>
void CTCircListVal<Typ>::RemoveAll()
{
    FirstLoc = LastLoc = Count = 0;
}

template<typename Typ>
void CTCircListVal<Typ>::RemoveFirst()
{
    if (Count > 0)
    {
        Count -= 1;
        if (Count > 0)
        {
            FirstLoc += 1;
            if (FirstLoc >= Capacity) FirstLoc = 0;
        }
    }
}

template<typename Typ>
bool CTCircListVal<Typ>::AddEntry(Typ *p_entry)
{
    if (IsFull()) return(false);

    if (Count)
    {
        LastLoc += 1;
        if (LastLoc >= Capacity) LastLoc = 0;
    }
    p_CircList[LastLoc] = *p_entry;
    Count += 1;

    return(true);
}

// get ptr to next open spot
template<typename Typ>
Typ* CTCircListVal<Typ>::GetNextPtr(bool incptr)
{
    uint32 nextloc;

    if (IsFull()) return(nullptr);

    nextloc = LastLoc;
    if (Count > 0)
    {
        nextloc += 1;
        if (nextloc >= Capacity) nextloc = 0;
    }

    if (incptr)
    {
        Count += 1;
        LastLoc = nextloc;
    }

    return(&(p_CircList[nextloc]));
}

template<typename Typ>
Typ* CTCircListVal<Typ>::GetFirst(bool remove)
{
    uint32 retind;

    if (Count <= 0) return(nullptr);

    retind = FirstLoc;
    if (remove)
    {
        Count -= 1;
        if (Count > 0)
        {
            FirstLoc += 1;
            if (FirstLoc >= Capacity) FirstLoc = 0;
        }
    }

    return(&(p_CircList[retind]));
}

template<typename Typ>
Typ* CTCircListVal<Typ>::GetLast() const
{
    if (Count <= 0) return(nullptr);

    return(&(p_CircList[LastLoc]));
}

template<typename Typ>
Typ* CTCircListVal<Typ>::GetAt(uint32 ind) const
{
    uint32 rawind;

    assert(Count > 0 && "In CTCircListVal<Typ>::GetAt");
    assert(ind < Count && "In CTCircListVal<Typ>::GetAt");

    rawind = FirstLoc + ind;

    if (rawind >= Capacity) rawind -= Capacity;

    return(&(p_CircList[rawind]));
}

template<typename Typ>
bool CTCircListVal<Typ>::IsFull() const
{
    if (Count >= Capacity) return(true);

    return(false);
}

template<typename Typ>
uint32 CTCircListVal<Typ>::GetCount() const
{
    return(Count);
}

//template<typename Typ>
//class CVecOfPtrs : public std::vector<Typ>
//{
//public:
//    bool Modified, DelOnRemove;

//    CVecOfPtrs(uint32 capacity=5);
//    virtual ~CVecOfPtrs();

//    void Add(Typ *pitem);
//    bool InsertAt(Typ *pitem, uint32 ind);
//    bool RemoveAt(uint32 ind);
//    void RemoveAll();
//    bool ReplaceAt(Typ *pitem, uint32 ind);
//    Typ* GetAt(uint32 ind) const;
//    Typ* GetLast() const;
//    uint32 GetCount() const;
//    uint32 GetCapacity() const;
//    bool SetCapacity(uint32 capacity);
//    void SetEqualTo(const CVecOfPtrs<Typ> &rhs);
//    void Swap(uint32 i1, uint32 i2);
//};


//template<typename Typ>
//CVecOfPtrs<Typ>::CVecOfPtrs(uint32 capacity)
//{
//    Modified = false;
//    DelOnRemove = true;
//}

//template<typename Typ>
//CVecOfPtrs<Typ>::~CVecOfPtrs()
//{
//    RemoveAll();
//    //delete [] List;
//}

//template<typename Typ>
//void CVecOfPtrs<Typ>::Add(Typ *pitem)
//{
//    //    if (Count == Capacity) IncreaseCapacity();
//    //    List[Count] = pitem;
//    //    Count++;
//    Modified = true;
//}

//template<typename Typ>
//bool CVecOfPtrs<Typ>::InsertAt(Typ *pitem, uint32 ind)
//{
//    assert(Count > 0 && "In CTList<Typ>::InsertAt");
//    assert(ind < Count && "In CTList<Typ>::InsertAt");

//    //    if (ind > Count) return(false);

//    //    if (Count == Capacity)
//    //        IncreaseCapacity();

//    //    for(uint32 i=Count-1; i>=ind; i--)
//    //        List[i+1] = List[i];

//    //    List[ind] = pitem;
//    //    Count++;
//    Modified = true;
//    return(true);
//}

//template<typename Typ>
//bool CVecOfPtrs<Typ>::RemoveAt(uint32 ind)
//{
//    assert(Count > 0 && "In CTList<Typ>::RemoveAt");
//    assert(ind < Count && "In CTList<Typ>::RemoveAt");

//    //    if (ind >= Count) return(false);

//    if (DelOnRemove)
//    {
//        //        if (List[ind] != nullptr)
//        //            delete List[ind];
//        //        List[ind] = nullptr;
//    }

//    //    for(uint32 i1=ind; i1<Count-1; i1++)
//    //        List[i1] = List[i1+1];

//    //    Count--;
//    Modified = true;
//    return(true);
//}

//template<typename Typ>
//void CVecOfPtrs<Typ>::RemoveAll()
//{
//    //    if (Count > 0) Modified = true;

//    //    if (DelOnRemove)
//    //        for (uint32 i=0; i < Count; i++)
//    //            delete List[i];

//    //    Count=0;
//    //    for(uint32 i=0; i<Capacity; i++) List[i] = nullptr;
//}

//template<typename Typ>
//bool CVecOfPtrs<Typ>::ReplaceAt(Typ *pitem, uint32 ind)
//{
//    //    assert(Count > 0 && "In CTList<Typ>::ReplaceAt");
//    //    assert(ind < Count && "In CTList<Typ>::ReplaceAt");

//    //    if (ind >= Count) return(false);

//    //    if (DelOnRemove)
//    //        delete List[ind];

//    //    List[ind] = pitem;
//    Modified = true;
//    return(true);
//}

//template<typename Typ>
//Typ* CVecOfPtrs<Typ>::GetAt(uint32 ind) const
//{
//    //    assert(Count > 0 && "In CTList<Typ>::GetAt");
//    //    assert(ind < Count && "In CTList<Typ>::GetAt");

//    //    if (Count == 0) return(nullptr);

//    //    if (ind >= Count) ind = Count-1;

//    return(this->at(ind));
//}

//template<typename Typ>
//Typ* CVecOfPtrs<Typ>::GetLast() const
//{
//    if (this->size() == 0)
//        return(nullptr);

//    return(this->at(this->size()-1));
//}

//template<typename Typ>
//uint32 CVecOfPtrs<Typ>::GetCount() const
//{
//    return(this->size());
//}

//template<typename Typ>
//void CVecOfPtrs<Typ>::SetEqualTo(const CVecOfPtrs<Typ> &rhs)
//{
//    Typ *p_item;

//    RemoveAll();
//    for(uint32 i=0; i<rhs.size(); i++)
//    {
//        p_item = new Typ();
//        *p_item = *(rhs.GetAt(i));
//        Add(p_item);
//    }

//    Modified = rhs.Modified;
//}

//template<typename Typ>
//void CVecOfPtrs<Typ>::Swap(uint32 i1, uint32 i2)
//{
//    Typ *pbuf;

//    pbuf = this->at(i1);
//    this->at(i1) = this->at(i2);
//    this->at(i2) = pbuf;
//}
