
#include "MyCString.h"
#include <stdarg.h>
#include <string.h>

CColorRef::CColorRef()
{
	Red = Green = Blue = 0;
}

CColorRef::CColorRef(uint32 red, uint32 green, uint32 blue)
{
	Red = red;
	Green = green;
	Blue = blue;
}

CColorRef::~CColorRef()
{
}

void CColorRef::SetColor(uint32 red, uint32 green, uint32 blue)
{
	Red = red;
	Green = green;
	Blue = blue;
}

bool CColorRef::operator==(const CColorRef& rhs) const
{
	if (rhs.Red == Red && rhs.Green == Green && rhs.Blue == Blue)
		return(true);

	return(false);
}

CErrorInfo::CErrorInfo()
{
	ErrorCnt = 0;
	Reset();
}

CErrorInfo::~CErrorInfo()
{
}

void CErrorInfo::Reset()
{
	ErrorMsg.clear();
	ErrorFlag = false;
	ErrorId = 0;
}

void CErrorInfo::Error(c_charptr errormsg, uint32 errorid)
{
    CMyCString buf;

	buf = errormsg;
	Error(buf, errorid);
}

void CErrorInfo::Error(std::string errormsg, uint32 errorid)
{
	ErrorCnt += 1;

	if (ErrorFlag == false)
	{
		ErrorFlag = true;
		ErrorId = errorid;
		ErrorMsg = errormsg;
	}
}

std::string CErrorInfo::ToString() const
{
    std::string retstr;

    if (ErrorFlag)
    {
        retstr = "Error: ";
        retstr += ErrorMsg.c_str();
    }
    else
        retstr = "Error: None";

    return(retstr);
}

//////////////////////////////////////////////////////////////////////////////////

CMyCString::CMyCString()
{
    SType = 0;
    ItemNum = UINT32_MAX;
    SEnabled = true;
    RefCnt = 0;
}

CMyCString::CMyCString(CCMyCStringRef cref)
{
    *this = cref;
}

CMyCString::CMyCString(c_charptr p_str)
{
    SType = 0;
    ItemNum = UINT32_MAX;
    SEnabled = true;
    RefCnt = 0;

    *this = p_str;
}

CMyCString::CMyCString(std::string stdstr)
{
    SType = 0;
    ItemNum = UINT32_MAX;
    SEnabled = true;
    RefCnt = 0;

    *this = stdstr;
}

CMyCString::CMyCString(QString qstr)
{
    SType = 0;
    ItemNum = UINT32_MAX;
    SEnabled = true;
    RefCnt = 0;

    *this = qstr;
}

CMyCString::~CMyCString()
{
}

uint32 CMyCString::GetLength() const
{
    return((uint32)length());
}

char CMyCString::GetAt(uint32 ind) const
{
    assert(GetLength() > 0 && "In CMyCString::GetAt");
    assert(ind < GetLength() && "In CMyCString::GetAt");

    if (ind >= GetLength()) return(UINT8_MAX);

	return(at(ind));
}

bool CMyCString::IsEmpty() const
{
	return(empty());
}

void CMyCString::Empty()
{
	clear();
}

void CMyCString::Format(c_charptr formatstr, ...)
{
	va_list vl;
	char *p_buf;
    uint32 size;

	va_start(vl, formatstr);
	size = vsnprintf(0, 0, formatstr, vl) + sizeof('\0');
	va_end(vl);

	p_buf = new char[size];

	va_start(vl, formatstr);
    vsnprintf(p_buf, size, formatstr, vl);
	va_end(vl);

	*this = p_buf;

	delete[] p_buf;
}

CMyCStringRef CMyCString::operator=(c_charptr str)
{
    uint32 len;

    Empty();
    len = (uint32)strlen(str);

    for (uint32 i=0; i < len; i++)
        *this += str[i];

    return(*this);
}

CMyCStringRef CMyCString::operator=(const std::string &rhs)
{
    uint32 len;

    Empty();
    len = (uint32)rhs.length();

    for (uint32 i=0; i < len; i++)
        *this += rhs.at(i);

    return(*this);
}

CMyCStringRef CMyCString::operator=(const QString &rhs)
{
    *this = rhs.toStdString();

    return(*this);
}

CMyCStringRef CMyCString::operator=(CCMyCStringRef rhs)
{
    uint32 len;

    Empty();
    len = (uint32)rhs.length();

    for (uint32 i=0; i < len; i++)
        *this += rhs.at(i);

    SType = rhs.SType;
    ItemNum = rhs.ItemNum;
    SEnabled = rhs.SEnabled;

    return(*this);
}

// Finds the index of the numth instance of 'tar' char that's not inside (), [] or {}.
// If numth instance doesn't exist, returns UINT32_MAX.
uint32 CMyCString::FindFree(const char tar, uint32 num) const
{
    uint32 loc, cnt=0, last, caught;
    int32 parenstate=0, cbstate=0, sbstate=0;
    char ch;

    last = GetLength()-1;
    loc = UINT32_MAX;
    for(uint32 i=0; i<=last && loc == UINT32_MAX; i++)
    {
        caught = 0;
        ch = GetAt(i);

        if ( ch==tar && parenstate==0 && cbstate==0 && sbstate==0)
        {
            if (++cnt == num)
                loc = i;
            caught = 1;
        }
        else if (ch == '(')
            parenstate++;
        else if (ch == ')')
            parenstate--;
        else if (ch == '[')
            sbstate++;
        else if (ch == ']')
            sbstate--;
        else if (ch == '{')
            cbstate++;
        else if (ch == '}')
            cbstate--;

        if (parenstate < 0) parenstate = 0;
        if (sbstate < 0) sbstate = 0;
        if (cbstate < 0) cbstate = 0;

        if (!caught)
            if ( ch==tar && parenstate==0 && cbstate==0 && sbstate==0)
            {
                if (++cnt == num)
                    loc = i;
            }
    }

    return(loc);
}

uint32 CMyCString::FindMatching(uint32 openind) const
{
    uint32 loc, cnt=0, last;
    char opench, closech, ch;

    loc = UINT32_MAX;
    last = GetLength()-1;

    if (openind > last) return(UINT32_MAX);

    opench = GetAt(openind);
    if (opench == '(')
        closech = ')';
    else if (opench == '[')
        closech = ']';
    else if (opench == '{')
        closech = '}';
    else
        return(UINT32_MAX);

    for(uint32 i=openind+1; i<=last && loc == UINT32_MAX; i++)
    {
        ch = GetAt(i);

        if (ch==closech && cnt==0)
            loc = i;
        else if (ch == opench)
            cnt++;
        else if (ch == closech)
            cnt--;
    }

    return(loc);
}

// Only counts delimiters that are outside of (), [] and {}.  ie free
CMyCString CMyCString::GetSubStrFree(uint32 num, char delimiter) const
{
    CMyCString retstr;
    uint32 delimcount=0, ind=0, len;
    uint32 parenstate, cbstate, sbstate;
    bool caught;
    char ch;

    len = GetLength();
    parenstate = cbstate = sbstate = 0;
    for(ind=0; (delimcount < num-1) && (ind < len); ind++)
    {
        caught = false;
        ch = GetAt(ind);
        if (ch==delimiter && parenstate==0 && cbstate==0 && sbstate==0)
        {
            delimcount++;
            caught = true;
        }
        else if (ch == '(')
            parenstate++;
        else if (ch == ')')
            parenstate--;
        else if (ch == '[')
            sbstate++;
        else if (ch == ']')
            sbstate--;
        else if (ch == '{')
            cbstate++;
        else if (ch == '}')
            cbstate--;

        // if delimiter is a closing paren, brace, or bracket, it'll get caught here.
        // if it's an opening one, it'll get caught above.
        if (!caught)
            if (ch==delimiter && parenstate==0 && cbstate==0 && sbstate==0)
                delimcount++;
    }

    parenstate = cbstate = sbstate = 0;
    for(; (delimcount < num) && (ind < len); ind++)
    {
        caught = false;
        ch = GetAt(ind);
        if (ch==delimiter && parenstate==0 && cbstate==0 && sbstate==0)
        {
            delimcount++;
            caught = true;
        }
        else if (ch == '(')
            parenstate++;
        else if (ch == ')')
            parenstate--;
        else if (ch == '[')
            sbstate++;
        else if (ch == ']')
            sbstate--;
        else if (ch == '{')
            cbstate++;
        else if (ch == '}')
            cbstate--;

        // if delimiter is a closing paren, brace, or bracket, it'll get caught here.
        // if it's an opening one, it'll get caught above.
        if (!caught)
            if (ch==delimiter && parenstate==0 && cbstate==0 && sbstate==0)
                delimcount++;

        if (delimcount < num)
            retstr = retstr + ch;
    }

    return(retstr);
}

void CMyCString::ToLower()
{
    for (uint32 i = 0; i < GetLength(); i++)
		if (at(i) >= 'A' && at(i) <= 'Z')
			replace(i, 1, 1, at(i) - ('A' - 'a'));
}

void CMyCString::ToUpper()
{
    for (uint32 i = 0; i < GetLength(); i++)
		if (at(i) >= 'a' && at(i) <= 'z')
			replace(i, 1, 1, at(i) + ('A' - 'a'));
}

int32 CMyCString::CompareNoCase(const char *p_comparestr) const
{
    CMyCString buf;

	buf = p_comparestr;

	return(CompareNoCase(buf));
}

int32 CMyCString::CompareNoCase(const CMyCString &comparestr) const
{
    CMyCString buf1, buf2;
    int32 retval;

	buf1 = *this;
	buf2 = comparestr;

	buf1.ToLower();
	buf2.ToLower();

	retval = buf1.compare(buf2);

	return(retval);
}

bool CMyCString::Contains(c_charptr psubstr, bool casesens, uint32 startpos) const
{
    if (casesens)
    {
        CMyCString mainstr2;
        CMyCString substr2;

        mainstr2 = *this;
        substr2 = psubstr;

        mainstr2.ToLower();
        substr2.ToLower();

        if (mainstr2.find(substr2, startpos) == std::string::npos)
            return(false);
    }
    else
    {
        if (find(psubstr, startpos) == std::string::npos)
            return(false);
    }

    return(true);
}

bool CMyCString::Contains(const CMyCString &substr1, bool casesens, uint32 startpos) const
{
    return(Contains(substr1.c_str(), casesens, startpos));
}

bool CMyCString::IsSubStrAt(CMyCString checkstr, bool casesens, uint32 checkloc) const
{
    CMyCString substr;

    if (GetLength() == 0) return(false);
    if (checkstr.GetLength() == 0) return(false);
    if (checkloc + checkstr.GetLength() > GetLength()) return(false);

    substr = GetSubStr1(checkloc, checkloc+checkstr.GetLength()-1);

    if (!casesens)
    {
        substr.ToLower();
        checkstr.ToLower();
    }

    if (substr != checkstr)
        return(false);

    return(true);
}

int32 CMyCString::ToInt(bool *p_convertok) const
{
    CMyCString buf;
    double retval=0;

	buf = *this;
	buf.StripChar(',');

    if (p_convertok) *p_convertok = true;

    try
    {
        retval = std::stoi(buf.c_str());
    }
    catch(...)
    {
        if (p_convertok) *p_convertok = false;
        retval = 0;
    }

    return(retval);
}

bool CMyCString::ToBool()
{
	if (CompareNoCase("true") == 0)
		return(true);

	if (CompareNoCase("false") == 0)
		return(false);

    if (std::stoi(this->c_str()))
		return(true);

	return(false);
}

double CMyCString::ToDouble(boolptr p_convertok) const
{
    CMyCString buf;
    double retval=0;

	buf = *this;
	buf.StripChar(',');

    if (p_convertok) *p_convertok = true;

    try
    {
        retval = stod(buf);
    }
    catch(...)
    {
        if (p_convertok) *p_convertok = false;
        retval = 0;
    }

    return(retval);
}

// Be sure and delete your array when you're done with it!
// And prob better off using c_str()
char* CMyCString::CharArray() const
{
    charptr p_char;

    if (IsEmpty()) return(nullptr);

	p_char = new char[GetLength() + 1];

    for (uint32 i = 0; i < GetLength(); i++)
		p_char[i] = at(i);

	p_char[GetLength()] = 0;

	return(p_char);
}

void CMyCString::InsertChar(uint32 idx, const char appchar)
{
    insert(idx, 1, appchar);
}

void CMyCString::InsertStr(uint32 idx, const char *appstr)
{
    insert(idx, appstr);
}

void CMyCString::InsertStr(uint32 idx, CMyCString &appstr)
{
    insert(idx, appstr.c_str());
}

void CMyCString::AppendChar(const char appchar)
{
    append(1, appchar);
}

void CMyCString::AppendStr(const char *appstr)
{
    append(appstr);
}

void CMyCString::AppendStr(const CMyCString &appstr)
{
    //Format("%s%s", c_str(), appstr.c_str());
    append(appstr.c_str());
}

void CMyCString::PrependChar(const char appchar)
{
    insert(0, 1, appchar);
}

void CMyCString::PrependStr(const char *appstr)
{
    insert(0, appstr);
}

void CMyCString::PrependStr(const CMyCString &appstr)
{
    insert(0, appstr.c_str());
}

void CMyCString::FromColorRef(CColorRef color)
{
	Empty();
	Format("%u:%u:%u", color.Red, color.Green, color.Blue);
}

CColorRef CMyCString::ToColorRef() const
{
    CMyCString colorcomp;
	CColorRef retcolor;

    colorcomp = GetSubStr2(1, ':', true); retcolor.Red = colorcomp.ToInt();
    colorcomp = GetSubStr2(2, ':', true); retcolor.Green = colorcomp.ToInt();
    colorcomp = GetSubStr2(3, ':', true); retcolor.Blue = colorcomp.ToInt();

	return(retcolor);
}

QString CMyCString::ToQString() const
{
    return(QString::fromStdString(*this));
}

void CMyCString::FromQString(const QString &qstr)
{
    QByteArray babuf;

    resize(qstr.length());

    babuf = qstr.toLatin1();
    for(uint32 i=0; i<GetLength(); i++)
        (*this)[i] = babuf[i];
}

bool CMyCString::IsLegalVarChar(char c, bool isfirstchar)
{
    if (isfirstchar)
    {
        if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') )
            return(1);

        return(false);
    }
    else
    {
        if ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c=='_'))
            return(true);

        return(false);
    }
}

bool CMyCString::IsLegalVarName() const
{
    uint32 i, len;

    len = GetLength();
    if (len == 0)
        return(false);
    for(i=0; i<len; i++)
    {
        if (!IsLegalVarChar(GetAt(i), !i))
            return(false);
    }
    return(true);
}

uint32 CMyCString::FindSubStr(c_charptr mstr, bool casesens, uint32 startpos) const
{
    size_type findpos;
    uint32 retval;

    if (casesens)
    {
        CMyCString mainstr2;
        CMyCString substr2;

        mainstr2 = *this;
        substr2 = mstr;

        mainstr2.ToLower();
        substr2.ToLower();

        findpos = mainstr2.find(substr2, startpos);
    }
    else
        findpos = find(mstr, startpos);

    if (findpos == std::string::npos)
        retval = UINT32_MAX;
    else
        retval = (uint32)findpos;

    return(retval);
}

void CMyCString::StripChar(char ch)
{
    for (uint32 i = 0; i < GetLength(); i++)
		if (GetAt(i) == ch)
		{
			StripMid(i, 1);
			i--;
		}
}

void CMyCString::StripComment()
{
    if (GetLength() < 2) return;

    for (uint32 i=0; i < GetLength()-1; i++)
        if (GetAt(i) == '/' && GetAt(i+1) == '/')
        {
			StripRight(GetLength() - i);
            break;
        }

    StripTrailingWhiteSpace();
}

uint32 CMyCString::StripWhiteSpace(bool incommentalso)
{
    uint32 cnt=0, len;
	bool incomment = false;
    charptr p_buf;
	char ch, prevch = 'a';  // can be anything but '/'

	len = GetLength();

    if (len == 0)
        return(0);

    p_buf = CharArray();
    for(uint32 i = 0; i < len; i++)
	{
		ch = GetAt(i);
		if ((ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') && (!incomment || incommentalso))
			cnt++;
		else if (cnt)
			p_buf[i - cnt] = p_buf[i];

		if (prevch == '/' && ch == '/')
			incomment = true;
		prevch = ch;
	}
	p_buf[len-cnt] = 0;

	*this = p_buf;
	delete[] p_buf;

	return(cnt);
}

void CMyCString::StripLeadingWhiteSpace()
{
    uint32 cnt=0, len;
	char ch;

	len = GetLength();
    for (uint32 i=0; i < len; i++)
	{
		ch = GetAt(i);
        if (ch==' ' || ch=='\t' || ch== '\n' || ch== '\r')
            cnt++;
        else
            i = len;
    }

    StripLeft(cnt);
}

void CMyCString::StripTrailingWhiteSpace()
{
    uint32 cnt=0, len;
    char ch;

    len = GetLength();
    for(uint32 i=len-1; i!=UINT32_MAX; i--)
    {
        ch = GetAt(i);
        if (ch==' ' || ch=='\t' || ch== '\n' || ch== '\r')
            cnt++;
        else
            i = 0;
    }

    StripRight(cnt);
}

void CMyCString::StripLeft(uint32 count)
{
    if (GetLength() <= count)
        Empty();
    else if (count > 0)
        erase(begin(), begin()+count);
}

void CMyCString::StripRight(uint32 count)
{
    if (GetLength() <= count)
		Empty();
	else if (count > 0)
        erase(end()-count, end());
}

void CMyCString::StripMid(uint32 start, uint32 count)
{
	erase(start, count);
}

void CMyCString::StripNameFromPath()
{
    uint32 i1, len;
    bool found=false;

	len = GetLength();
    for (i1 = len - 1; i1 != UINT32_MAX && !found; i1--)
        if (GetAt(i1) == '/')
            found = true;

    i1++;
	if (found)
        StripRight(len - i1);
}

bool CMyCString::StripTrailingNewlines()
{
    while(GetLength() && (GetAt(GetLength()-1) == '\n' || GetAt(GetLength()-1) == '\r') )
        StripRight();

    if (IsEmpty())
        return(false);

    return(true);
}

void CMyCString::GetComment(CMyCStringRef comment) const
{
    comment.Empty();

    for (uint32 i=0; i<GetLength()-1; i++)
        if (GetAt(i) == '/' && GetAt(i+1) == '/')
        {
            comment = GetSubStr1(i, GetLength()-1);
            comment.StripLeft(2);
            comment.StripLeadingWhiteSpace();
            comment.StripTrailingWhiteSpace();
        }
}

CMyCString CMyCString::GetNameFromPath() const
{
    uint32 len, found=0;
    CMyCString retstr;

	len = GetLength();
    for (uint32 i = len - 1; i != UINT32_MAX && !found; i--)
        if (GetAt(i) == '/')
			found = i + 1;


    for (uint32 i = found; i < len; i++)
		retstr += GetAt(i);

	return(retstr);
}

CMyCString CMyCString::GetFileExtension() const
{
    uint32 len, ind;
    bool found=false;
    CMyCString retstr;

    len = GetLength();
    for (uint32 i=len-1; i!=UINT32_MAX && !found; i--)
        if (GetAt(i) == '.')
        {
            found = true;
            ind = i;
        }

    if (found)
        for (uint32 i=ind+1; i<len; i++)
            retstr.AppendChar(GetAt(i));

    return(retstr);
}

// ex:  "hi,there,,folks"
// GetSubStr2(0, ',') returns ""
// GetSubStr2(1, ',') returns hi
// GetSubStr2(2, ',') returns there
// GetSubStr2(3, ',') returns folks
// GetSubStr2(4, ',') returns ""
// GetSubStr2(-1, ',') returns "folks"
// GetSubStr2(-2, ',') returns "there"
// GetSubStr2(-3, ',') returns "hi"
// GetSubStr2(-4, ',') returns ""
CMyCString CMyCString::GetSubStr2(int32 num, char delimiter, bool stripwhite) const
{
    CMyCString retstr;
    uint32 delimcount, ind;

    assert(num != 0 && "In CMyCString::GetSubStr2");
    assert(GetLength() > 0 && "In CMyQString::GetSubStr2");

    if (num == 0 || GetLength() == 0)
        retstr = "";
    else if (num < 0)
    {
        ind = GetLength() - 1;

        while (ind != UINT32_MAX && GetAt(ind) != delimiter)
			ind--;

		ind += 1;
        for (uint32 i = ind; i < GetLength(); i++)
			retstr += GetAt(i);
    }
    else // num > 0
	{
		delimcount = 0;
		ind = 0;

        while ((delimcount < (uint32)num - 1) && (ind < GetLength()))
		{
			if (GetAt(ind) == delimiter)
			{
				delimcount++;
				while (GetAt(ind) == delimiter)  // there could be multiple spaces for example
					ind++;
			}
			else
				ind++;
		}

        while ((ind < GetLength()) && GetAt(ind) != delimiter)
		{
			retstr += GetAt(ind);
			ind++;
		}
	}

	if (stripwhite)
		retstr.StripWhiteSpace();

	return(retstr);
}

// used with CBaseMat's FromString
CMyCString CMyCString::GetSubStr3(int32 num, char delimiter1, char delimiter2, uint32ref stopcause) const
{
    CMyCString retstr;
    uint32 delimcount=0, ind=0, len;

	if (num <= 0) return(retstr);

	len = GetLength();

    while ((delimcount < (uint32)num - 1) && (ind < len))
	{
		if (GetAt(ind) == delimiter1 || GetAt(ind) == delimiter2)
		{
			delimcount++;
			while (GetAt(ind) == delimiter1 || GetAt(ind) == delimiter2)  // there could be multiple spaces for example
				ind++;
		}
		else
			ind++;
	}

	while ((ind < len) && GetAt(ind) != delimiter1 && GetAt(ind) != delimiter2)
	{
		retstr += GetAt(ind);
		ind++;
	}

	if (ind >= len)
		stopcause = 3;
	else if (GetAt(ind) == delimiter1)
		stopcause = 1;
	else if (GetAt(ind) == delimiter2)
		stopcause = 2;
	else
		stopcause = 3;

	retstr.StripWhiteSpace();

	return(retstr);
}

CMyCString CMyCString::GetLine(uint32& curidx) const
{
    CMyCString retstr;

    while(curidx < GetLength() && GetAt(curidx) != '\n' && GetAt(curidx) != '\r')
    {
        retstr.AppendChar(GetAt(curidx));
        curidx++;
    }

    while(curidx < GetLength() && (GetAt(curidx) == '\n' || GetAt(curidx) == '\r'))
        curidx++;

    return(retstr);
}

void CMyCString::Split(std::vector<std::string> &substrs, const std::string delims) const
{
    uint32 idx=0, start, stop;
    bool lookingfordelim=true;

    substrs.clear();

    while(idx < size())
    {
        if (lookingfordelim)
        {
            start = idx;
            while(idx < size() && delims.find(at(idx)) == std::string::npos)
                idx++;
            stop = idx-1;
            substrs.push_back(substr(start, stop-start+1));
            lookingfordelim = false;
        }
        else
        {
            while(idx < size() && delims.find(at(idx)) != std::string::npos)
                idx++;
            lookingfordelim = true;
        }
    }
}

void CMyCString::Join(const std::vector<std::string> &substrs, char delim)
{
    clear();

    for(uint32 i=0; i<substrs.size(); i++)
    {
        append(substrs[i]);
        if (i<substrs.size()-1)
            append(1, delim);
    }
}

// returns zero-offset chars 'start' thru 'stop'.
// ex: "help write"
// GetSubStr(1,5)   returns "elp w"
// GetSubStr(3,100) returns "p write"
CMyCString CMyCString::GetSubStr1(uint32 start, uint32 stop) const
{
    if (GetLength() == 0 || start >= GetLength() || start > stop)
        return("");

    if (stop >= GetLength()) stop = GetLength()-1;
    if (start > stop) start = stop;

    return(substr(start, stop-start+1));
}

////////////////////////////////////////////////////////////////////////////////////////////////

CMyCStringList::CMyCStringList(uint32 capacity)
{
    Modified = false;
	Count = 0;
	Capacity = capacity;
	if (Capacity < 1) Capacity = 1;

    List = new CMyCString*[Capacity];
}

CMyCStringList::~CMyCStringList()
{
	RemoveAll();
	delete [] List;
}

void CMyCStringList::IncreaseCapacity()
{
    CMyCString** newlist;

	Capacity *= 2;
    newlist = new CMyCString*[Capacity];
    for(uint32 i=0; i<Count; i++) newlist[i] = List[i];
	delete [] List;
	List = newlist;
}

void CMyCStringList::Add(CMyCString *pstring)
{
	if (Count == Capacity) IncreaseCapacity();
    pstring->RefCnt += 1;
	List[Count] = pstring;
	Count++;
    Modified = true;
}

bool CMyCStringList::InsertAt(CMyCString *pstring, uint32 ind)
{
    assert(Count > 0 && "In CMyCStringList::InsertAt");
    assert(ind < Count && "In CMyCStringList::InsertAt");

    if (ind > Count) return(false);

	if (Count == Capacity)
		IncreaseCapacity();

    for(uint32 i=Count-1; i>=ind; i--)
		List[i+1] = List[i];

    pstring->RefCnt += 1;
	List[ind] = pstring;
	Count++;
    Modified = true;
	return(true);
}

bool CMyCStringList::RemoveAt(uint32 ind)
{
    assert(Count > 0 && "In CMyCStringList::RemoveAt");
    assert(ind < Count && "In CMyCStringList::RemoveAt");

    if (ind >= Count) return(false);

    if (List[ind]->RefCnt > 0) List[ind]->RefCnt -= 1;
    if (List[ind]->RefCnt == 0) delete List[ind];

    for(uint32 i=ind; i<Count-1; i++)
		List[i] = List[i+1];
	Count--;
    Modified = true;
	return(true);
}

void CMyCStringList::RemoveAll()
{
    if (Count == 0) return;

    for(uint32 i=0; i<Count; i++)
    {
        if (List[i]->RefCnt > 0) List[i]->RefCnt -= 1;
        if (List[i]->RefCnt == 0)
            delete List[i];
    }

	Count=0;
    Modified = true;
}

bool CMyCStringList::ReplaceAt(CMyCString *pstring, uint32 ind)
{
    assert(Count > 0 && "In CMyCStringList::ReplaceAt");
    assert(ind < Count && "In CMyCStringList::ReplaceAt");

    if (ind >= Count) return(false);

    if (List[ind]->RefCnt > 0) List[ind]->RefCnt -= 1;
    if (List[ind]->RefCnt == 0) delete List[ind];

    pstring->RefCnt += 1;
    List[ind] = pstring;
    Modified = true;
	return(true);
}

CMyCStringPtr CMyCStringList::GetAt(uint32 ind) const
{
    assert(Count > 0 && "In CMyCStringList::GetAt");
    assert(ind < Count && "In CMyCStringList::GetAt");

    if (ind >= Count) ind = Count-1;

	return(List[ind]);
}

// This could mess-up ref-counting
// CMyCStringPtr& CMyCStringList::operator[](uint32 ind) const
// {
//     assert(Count > 0 && "In CMyCStringList::operator[]");
//     assert(ind < Count && "In CMyCStringList::operator[]");

//     if (ind >= Count) ind = Count-1;

//     CMyCStringPtr& retval = List[ind];
//     return(retval);
// }

uint32 CMyCStringList::GetCount() const
{
	return(Count);
}

uint32 CMyCStringList::GetCapacity() const
{
	return(Capacity);
}

bool CMyCStringList::SetCapacity(uint32 capacity)
{
    uint32 newcnt;
    CMyCString** newlist;

    if (capacity < 1) return(false);

    newlist = new CMyCString*[capacity];
    if (Count <= capacity)
        newcnt = Count;
    else
        newcnt = capacity;

    for(uint32 i=0; i<newcnt; i++)
        newlist[i] = List[i];

    for(uint32 i=newcnt; i<Count; i++)
    {
        if (List[i]->RefCnt > 0) List[i]->RefCnt -= 1;
        if (List[i]->RefCnt == 0) delete List[i];
    }

    delete [] List;
    List = newlist;
    Count = newcnt;
    Capacity = capacity;
    return(true);
}

void CMyCStringList::SetEqualTo(CCMyCStringListRef stringlist)
{
    CMyCString *p_string;

	RemoveAll();
    for(uint32 i=0; i<stringlist.GetCount(); i++)
	{
        p_string = new CMyCString();
        *p_string = *(stringlist.GetAt(i));
        p_string->RefCnt = 0;  // jic above copy set it higher
        Add(p_string);
	}

    Modified = stringlist.Modified;
}

void CMyCStringList::Swap(uint32 i1, uint32 i2)
{
    CMyCString *pbuf;

	pbuf = List[i1];
	List[i1] = List[i2];
	List[i2] = pbuf;
}

CMyCString CMyCStringList::ToSingleStr(bool addcrs, bool adddblslashes) const
{
    CMyCString retval;

    for(uint32 i=0; i<GetCount(); i++)
    {
        if (adddblslashes && !GetAt(i)->SEnabled)
            retval.AppendStr("// ");

        retval.AppendStr(*GetAt(i));

        if (addcrs && i<GetCount()-1)
            retval.AppendStr("\n");
    }

    return retval;
}

void CMyCStringList::FromSingleStr(CCMyCStringRef singlestr, bool stripleadingwhite, bool setenables)
{
    uint32 curidx=0;
    CMyCString linestr;
    CMyCStringPtr p_str;

    RemoveAll();
    while(curidx < singlestr.GetLength())
    {
        linestr = singlestr.GetLine(curidx);
        if (stripleadingwhite)
            linestr.StripLeadingWhiteSpace();
        if (setenables && linestr.GetSubStr1(0,1) == "//")
        {
            linestr.SEnabled = false;
            linestr.StripLeft(2);
            linestr.StripLeadingWhiteSpace();
        }

        if (!linestr.IsEmpty())
        {
            p_str = new CMyCString();
            *p_str = linestr;
            Add(p_str);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
