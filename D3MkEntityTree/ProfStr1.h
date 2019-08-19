
// +--------------------------------------------------
// |
// | ProfStr1.h
// |
// | D3MkEntityTree : Copyright (c) 2004, kentaro-k.21
// |

#pragma once

#include <stdio.h>

#if _MFC_VER >= 0x0600

#ifdef _DEBUG

#ifndef _VERIFY
#define _VERIFY(T) ASSERT(T)
#endif

#ifndef _ASSERT
#define _ASSERT(T) ASSERT(T)
#endif

#else

#ifndef _VERIFY
#define _VERIFY(T) ((void)(T))
#endif

#ifndef _ASSERT
#define _ASSERT(T) 
#endif

#endif // _DEBUG

#endif // _MFC_VER >= 0x0600

class CProfStr
{
	// 
	class _NO_MEMBERWISE_COPY
	{
		// 
		_NO_MEMBERWISE_COPY(const _NO_MEMBERWISE_COPY &);
		// 
		void operator =(const _NO_MEMBERWISE_COPY &);

	protected:
		// 
		_NO_MEMBERWISE_COPY() { }

	};
	// 
	class _MARKER_
	{
	public:
		// 
		size_t iMark;

		// 
		bool operator ==(const _MARKER_ &s) const
		{
			return iMark == s.iMark;
		}
	};
	// 
	class _MEM_
	{
	public:
		// 
		BYTE *pData;
		// 
		size_t nDataLen;

		// 
		_MEM_() { pData = NULL, nDataLen = 0; }
	};
	// 
	class _MALLOC_ : public _MEM_, _NO_MEMBERWISE_COPY
	{
	public:
		// 
		_MALLOC_()
		{
			pData = NULL;
			nDataLen = 0;
		}
		// 
		~_MALLOC_()
		{
			Close();
		}
		// 
		void Close()
		{
			if (pData) free(pData), pData = NULL, nDataLen = 0;
		}
		// 
		bool Create(size_t n)
		{
			Close();
			if (pData = (BYTE *)malloc(n)) {
				nDataLen = n;
				return true;
			}
			return false;
		}
	};
	// 
	class _EXTRACTOR_
	{
	public:
		// 
		virtual _MARKER_ GetMarker() = NULL;
		// 
		virtual bool Ex(_MARKER_ &mark0, _MARKER_ &mark1, _MEM_ &m) = NULL;
	};
	// 
	class _MEMST_ : public _MEM_, public _EXTRACTOR_
	{
	public:
		// 
		size_t iDataPos;

		// 
		_MEMST_() { }
		// 
		explicit _MEMST_(_MEM_ &m)
		{
			pData = m.pData;
			iDataPos = 0;
			nDataLen = m.nDataLen;
		}
		// 
		virtual _MARKER_ GetMarker()
		{
			_MARKER_ m;

			m.iMark = iDataPos;
			return m;
		}
		// 
		virtual bool Ex(_MARKER_ &mark0, _MARKER_ &mark1, _MEM_ &m)
		{
			if (nDataLen < mark0.iMark)
				return false;
			if (mark1.iMark < mark0.iMark)
				return false;

			m.pData = pData +mark0.iMark;
			m.nDataLen = mark1.iMark -mark0.iMark;
			return true;
		}
	};
	// 
	class _FILE_ : _NO_MEMBERWISE_COPY
	{
		// 
		FILE *f;

		// 
		size_t GetFileSize()
		{
			size_t i = ftell(f);
			fseek(f, 0, SEEK_END);
			size_t n = ftell(f);
			fseek(f, i, SEEK_SET);

			return n;
		}

	public:
		// 
		_FILE_()
			: f(NULL)
		{

		}
		// 
		~_FILE_()
		{
			Close();
		}
		// 
		bool Open(LPCSTR psz, bool fRead)
		{
			Close();
			if (false
				|| (f = fopen(psz, fRead ? "r+b" : "w+b")) != NULL
			) {
				return true;
			}
			return false;
		}
		// 
		void Close()
		{
			if (f) fclose(f), f = NULL;
		}
		// 
		bool Read(_MALLOC_ &m)
		{
			size_t n = GetFileSize();
			if (!m.Create(n))
				return false;
			if (fseek(f, 0, SEEK_SET) != 0)
				return false;
			if (n == 0)
				return true;
			if (fread(m.pData, n, 1, f) != 1)
				return false;
			return true;
		}
		// 
		bool Write(_MEM_ &m, bool fSeekSet = false)
		{
			if (fSeekSet && fseek(f, 0, SEEK_SET) != 0)
				return false;
			if (m.nDataLen == 0)
				return true;
			if (fwrite(m.pData, m.nDataLen, 1, f) != 1)
				return false;
			return true;
		}
	};
	// 
	enum elemType {
		elemUnknown,
		elemSection,
		elemKey,
		elemComment,
		elemNewline,
	};
	// 
	class _ELEM_
	{
	public:
		// 
		int elemType;
		// 
		_MARKER_ first, last, nl;
		_MARKER_ key0, key1;
		_MARKER_ val0, val1;
	};
	// 
	class _ELEM_SECTION_
	{
	public:
		// 
		_MARKER_ first, last;
	};
	// 
	enum commandType {
		commandReplaceSection,
		commandReplaceKey,
		commandQuerySection,
		commandQuerySectionNames,
		commandQueryKey,
	};
	// 
	class _COMMAND_
	{
	public:
		// 
		int commandType;
		// 
		_MEM_ ma, mk, mv;
		_MEMST_ mr;
	};
	// 
	class _PARSER_
	{
		// 
		enum skipOpts {
			skipNo = 0,
			skipSpc = 1,
			skipNotBR = 2,
		};

		// 
		_MEMST_ &f;

	public:
		// 
		_PARSER_(_MEMST_ &f)
			: f(f)
		{

		}
		// 
		bool IsEnd()
		{
			return Cur() < 0;
		}
		// 
		_MARKER_ GetMarker()
		{
			return f.GetMarker();
		}
		// 
		bool NextLine()
		{
			Skip_NotBR();
			Parse_BR();

			return !IsEnd();
		}

		// 
		bool Parse_1st(_ELEM_ &t)
		{
			t.first = GetMarker();
			Skip_spc();
			t.elemType = CProfStr::elemUnknown;

			if (false);
			else if (IsMatch('[', skipNo)) {
				t.key0 = GetMarker();
				if (TestMatch(']', skipNotBR)) {
					t.key1 = GetMarker();
					t.elemType = CProfStr::elemSection;
				} else {

				}
			}
			else if (IsMatch(';', skipNo)) {
				t.elemType = CProfStr::elemComment;
			}
			else if (Is_BR(Cur())) {
				t.elemType = CProfStr::elemNewline;
			}
			else {
				t.key0 = GetMarker();
				if (TestMatch('=', skipNotBR)) {
					t.key1 = GetMarker();
					t.elemType = CProfStr::elemKey;

					Next();
					t.val0 = GetMarker();
					Skip_NotBR();
					t.val1 = GetMarker();
				} else {

				}
			}

			Skip_NotBR();
			t.last = GetMarker();
			NextLine();
			t.nl = GetMarker();
			return true;
		}
		// 
		bool Parse_section(_ELEM_SECTION_ &t)
		{
			t.first = GetMarker();
			if (TestMatch(0, skipNotBR)) {
				t.last = GetMarker();
				Next();
				return true;
			}
			t.last = GetMarker();
			Parse_BR();
			return true;
		}
		// 
		bool IsMatch(int c, int skip)
		{
			if (TestMatch(c, skip)) {
				Next();
				return true;
			}
			return false;
		}
		// 
		bool TestMatch(int c, int skip)
		{
			while (!IsEnd()) {
				int a = Cur();
				if (a == c) {
					return true;
				}
				if (skip == skipNo)
					break;
				if (skip & skipSpc) {
					if (!Is_spc(a))
						break;
				}
				if (skip & skipNotBR) {
					if (Is_BR(a))
						break;
				}
				Next();
			}
			return false;
		}

		// 
		void Skip_NotBR()
		{
			while (!IsEnd()) {
				if (Is_BR(Cur()))
					break;
				if (!Next())
					break;
			}
		}
		// 
		void Skip_spc()
		{
			while (!IsEnd()) {
				if (!Is_spc(Cur()))
					break;
				if (!Next())
					break;
			}
		}
		// 
		void Skip_ws()
		{
			while (!IsEnd()) {
				if (!Is_ws(Cur()))
					break;
				if (!Next())
					break;
			}
		}

	protected:
		// 
		bool Next()
		{
			if (f.iDataPos < f.nDataLen) {
				f.iDataPos++;
				return true;
			}
			return false;
		}
		// 
		int Cur()
		{
			int r = -1;
			if (f.iDataPos < f.nDataLen) {
				r = f.pData[f.iDataPos];
			}
			return r;
		}
		// 
		void Parse_BR()
		{
			int a = Cur();
			if (a == '\r') {
				if (Next()) {
					int a = Cur();
					if (a == '\n') {
						Next();
						return;
					}
				}
				return;
			}
			if (a == '\n') {
				Next();
				return;
			}
		}

		// 
		static bool Is_spc(int c)
		{
			switch (c) {
			case ' ':
			case '\t':
				return true;
			}
			return false;
		}
		// 
		static bool Is_ws(int c)
		{
			switch (c) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				return true;
			}
			return false;
		}
		// 
		static bool Is_BR(int c)
		{
			switch (c) {
			case '\r':
			case '\n':
				return true;
			}
			return false;
		}

	};
	// 
	struct _REPLACE_CHAIN_ : _NO_MEMBERWISE_COPY {
		// 
		_REPLACE_CHAIN_ *chain;
		// 
		_MARKER_ pos0, pos1;
		// 
		_MEM_ m;

		// 
		_REPLACE_CHAIN_()
			: chain(NULL)
		{

		}
		// 
		~_REPLACE_CHAIN_()
		{
			Close();
		}
		// 
		void Close()
		{
			if (chain) delete chain; chain = NULL;
		}
	};
	// 
	struct _REPLACE_BOARD_ : _NO_MEMBERWISE_COPY {
		// 
		_REPLACE_CHAIN_ *chain;

		// 
		_REPLACE_BOARD_()
			: chain(NULL)
		{

		}
		// 
		~_REPLACE_BOARD_()
		{
			Close();
		}
		// 
		void Close()
		{
			if (chain) delete chain; chain = NULL;
		}
	};

	// 
	_FILE_ f;
	// 
	_REPLACE_BOARD_ rc;
	// 
	_MALLOC_ fo;

	// 
	void AppendReplace(_MARKER_ &pos0, _MARKER_ &pos1, _MEM_ &m)
	{
		_REPLACE_CHAIN_ *p0 = new _REPLACE_CHAIN_();
		p0->pos0 = pos0;
		p0->pos1 = pos1;
		p0->m = m;
		_REPLACE_CHAIN_ *p1 = rc.chain;

		if (p1 == NULL) {
			rc.chain = p0;
		} else {
			while (p1->chain != NULL) p1 = p1->chain;

			p1->chain = p0;
		}
	}

	// 
	static bool CopyStr(_MEM_ &m0, _MEM_ &m1)
	{
		if (m0.nDataLen < m1.nDataLen +1)
			return false;
		memcpy(m0.pData, m1.pData, m1.nDataLen);
		m0.pData[m1.nDataLen] = 0;
		return true;
	}
	// 
	static bool AppendStr(_MEMST_ &f, _MEM_ &m1, bool fAppend0 = true)
	{
		size_t n0 = m1.nDataLen;
		size_t n1 = n0 +(fAppend0 ? 1 : 0);
		if (f.nDataLen -f.iDataPos < n1)
			return false;
		memcpy(f.pData +f.iDataPos, m1.pData, n0);
		if (fAppend0) {
			f.pData[f.iDataPos +n0] = 0;
		}
		f.iDataPos += n1;
		return true;
	}
	// 
	static bool AppendStr(_MEMST_ &f, LPCSTR pText, size_t nText, bool fAppend0 = true)
	{
		_MEM_ m;
		m.pData = (BYTE *)const_cast<LPSTR>(pText);
		m.nDataLen = nText;
		return AppendStr(f, m, fAppend0);
	}
	// 
	static int CompareStr(_MEM_ &m0, _MEM_ &m1)
	{
		int x = 0;
		size_t n;
		if (false);
		else if (m0.nDataLen < m1.nDataLen) {
			n = m0.nDataLen;
			x = -1;
		}
		else if (m0.nDataLen > m1.nDataLen) {
			n = m1.nDataLen;
			x = +1;
		}
		else {
			n = m0.nDataLen;
		}
		int r;
		r = _strnicmp((LPCSTR)m0.pData, (LPCSTR)m1.pData, n);
		if (r != 0)
			return r;
		if (x != 0)
			return x;
		return 0;
	}
	// 
	static bool Malloc_k_v_crlf(_MALLOC_ &m, _MEM_ &mk, _MEM_ &mv)
	{
		size_t n = mk.nDataLen + 1 + mv.nDataLen + 2;
		if (!m.Create(n))
			return false;
		_MEMST_ mr(m);
		AppendStr(mr, mk, false);
		AppendStr(mr, "=", 1, false);
		AppendStr(mr, mv, false);
		AppendStr(mr, "\r\n", 2, false);
		return true;
	}
	// 
	static bool Malloc_s_crlf_k_v_crlf(_MALLOC_ &m, _MEM_ &ma, _MEM_ &mk, _MEM_ &mv)
	{
		size_t n = (1 + ma.nDataLen + 1 + 2) + (mk.nDataLen + 1 + mv.nDataLen + 2);
		if (!m.Create(n))
			return false;
		_MEMST_ mr(m);
		AppendStr(mr, "[", 1, false);
		AppendStr(mr, ma, false);
		AppendStr(mr, "]", 1, false);
		AppendStr(mr, "\r\n", 2, false);
		AppendStr(mr, mk, false);
		AppendStr(mr, "=", 1, false);
		AppendStr(mr, mv, false);
		AppendStr(mr, "\r\n", 2, false);
		return true;
	}
	// 
	static bool Malloc_s_crlf(_MALLOC_ &m, _MEM_ &ma)
	{
		size_t n = (1 + ma.nDataLen + 1 + 2);
		if (!m.Create(n))
			return false;
		_MEMST_ mr(m);
		AppendStr(mr, "[", 1, false);
		AppendStr(mr, ma, false);
		AppendStr(mr, "]", 1, false);
		AppendStr(mr, "\r\n", 2, false);
		return true;
	}
	// 
	static bool Malloc_f(_MALLOC_ &m, LPCSTR psz)
	{
		size_t n = strlen(psz);
		if (!m.Create(n +1))
			return false;
		_MEMST_ mr(m);
		AppendStr(mr, psz, n);
		return true;
	}
	// 
	static size_t StrLen0(LPCSTR psz)
	{
		size_t n = 0;
		for (; ; ) {
			if (psz[0] == 0 && psz[1] == 0) {
				n += 2;
				break;
			}
			n++;
			psz++;
		}
		return n;
	}

	// 
	bool IsSection4Me(_EXTRACTOR_ &f, _ELEM_ &t, _MEM_ &ma)
	{
		if (t.elemType == elemSection) {
			_MEM_ ma0;
			if (f.Ex(t.key0, t.key1, ma0)) {
				if (CompareStr(ma, ma0) == 0) {
					return true;
				}
			}
		}
		return false;
	}
	// 
	bool IsKey4Me(_EXTRACTOR_ &f, _ELEM_ &t, _MEM_ &mk)
	{
		if (t.elemType == elemKey) {
			_MEM_ mk0;
			if (f.Ex(t.key0, t.key1, mk0)) {
				if (CompareStr(mk, mk0) == 0) {
					return true;
				}
			}
		}
		return false;
	}
	// 
	bool AppendStr(_EXTRACTOR_ &f, _MARKER_ &m0, _MARKER_ &m1, _MEMST_ &mr)
	{
		_MEM_ m;
		if (f.Ex(m0, m1, m)) {
			if (AppendStr(mr, m)) {
				return true;
			}
		}
		return false;
	}

	// 
	bool RewriteNow(_MEM_ &m)
	{
		size_t v0 = 0;
		size_t v3 = m.nDataLen;

		int i = 0;

		if (!OpenWrite())
			return false;

		_REPLACE_CHAIN_ *p0 = rc.chain;
		while (p0 != NULL) {
			size_t v1 = p0->pos0.iMark;
			size_t v2 = p0->pos1.iMark;

			_ASSERT(v0 <= v1);

			_MEM_ mm;
			mm.pData = m.pData +v0;
			mm.nDataLen = v1 -v0;

			if (!f.Write(mm, i == 0)) {
				return false;
			}
			i++;

			if (!f.Write(p0->m, false)) {
				return false;
			}

			v0 = v2;
			i++;

			p0 = p0->chain;
		}

		_MEM_ mm;
		mm.pData = m.pData +v0;
		mm.nDataLen = v3 -v0;

		if (!f.Write(mm, i == 0)) {
			return false;
		}

		return true;
	}
	// 
	void AppendSection(_MEMST_ &f, _MARKER_ last)
	{
		_MEM_ m1;
		m1.pData = (BYTE *)"\r\n";
		m1.nDataLen = 2;
		_PARSER_ o(f);
		for (; !o.IsEnd(); ) {
			_ELEM_SECTION_ t;
			_VERIFY(o.Parse_section(t));
			if (t.first == t.last)
				continue;
			_MEM_ m;
			_VERIFY(f.Ex(t.first, t.last, m));
			AppendReplace(last, last, m);
			AppendReplace(last, last, m1);
		}
	}

	// 
	bool Command(_COMMAND_ &c)
	{
		_MALLOC_ m;
		if (!f.Read(m))
			return false;
		rc.Close();
		_MEMST_ f;
		f.pData = m.pData;
		f.nDataLen = m.nDataLen;
		f.iDataPos = 0;
		_PARSER_ o(f);
		switch (c.commandType) {
		case commandQueryKey:
			{
				for (; !o.IsEnd(); ) {
					_ELEM_ t;
					o.Parse_1st(t);
					if (IsSection4Me(f, t, c.ma)) {
						for (; !o.IsEnd(); ) {
							_ELEM_ t;
							o.Parse_1st(t);
							if (t.elemType == elemSection)
								return false;
							if (IsKey4Me(f, t, c.mk)) {
								if (AppendStr(f, t.val0, t.val1, c.mr)) {
									return true;
								}
								return false;
							}
						}
					}
				}
				return false;
			}
		case commandQuerySection:
			{
				size_t w = 1;
				for (; !o.IsEnd(); ) {
					_ELEM_ t;
					o.Parse_1st(t);
					if (IsSection4Me(f, t, c.ma)) {
						for (; !o.IsEnd(); ) {
							_ELEM_ t;
							o.Parse_1st(t);
							if (t.elemType == elemSection)
								break;
							if (t.elemType == elemComment || t.elemType == elemNewline)
								continue;
							if (t.first == t.last)
								continue;
							if (!AppendStr(f, t.first, t.last, c.mr))
								return false;
							w = 0;
						}
						if (!AppendStr(c.mr, "\0", w))
							return false;
						return true;
					}
				}
				return false;
			}
		case commandQuerySectionNames:
			{
				size_t w = 1;
				for (; !o.IsEnd(); ) {
					_ELEM_ t;
					o.Parse_1st(t);
					if (t.elemType != elemSection)
						continue;
					if (!AppendStr(f, t.key0, t.key1, c.mr))
						return false;
					w = 0;
				}
				if (!AppendStr(c.mr, "\0", w))
					return false;
				return true;
			}
		case commandReplaceKey:
			{
				_MALLOC_ mm;
				if (!Malloc_k_v_crlf(mm, c.mk, c.mv))
					return false;

				for (; !o.IsEnd(); ) {
					_ELEM_ t;
					o.Parse_1st(t);
					if (IsSection4Me(f, t, c.ma)) {
						for (; !o.IsEnd(); ) {
							_ELEM_ t;
							o.Parse_1st(t);
							if (t.elemType == elemSection) {
								AppendReplace(t.first, t.first, mm);
								return RewriteNow(f);
							}
							if (IsKey4Me(f, t, c.mk)) {
								AppendReplace(t.first, t.nl, mm);
								return RewriteNow(f);
							}
						}
					}
				}
				if (!Malloc_s_crlf_k_v_crlf(mm, c.ma, c.mk, c.mv))
					return false;
				_MARKER_ last = o.GetMarker();
				AppendReplace(last, last, mm);
				return RewriteNow(f);
			}
		case commandReplaceSection:
			{
				_MALLOC_ ma;
				if (!Malloc_s_crlf(ma, c.ma))
					return false;

				_MEM_ m0;
				m0.pData = 0;
				m0.nDataLen = 0;
				for (; !o.IsEnd(); ) {
					_ELEM_ t;
					o.Parse_1st(t);
					if (IsSection4Me(f, t, c.ma)) {
						_MARKER_ first = o.GetMarker();
						for (; !o.IsEnd(); ) {
							_ELEM_ t;
							o.Parse_1st(t);
							if (t.elemType == elemSection) {
								first = t.first;
								break;
							}
							if (t.elemType == elemComment)
								continue;
							AppendReplace(t.first, t.nl, m0);
							first = t.nl;
						}
						AppendSection(_MEMST_(c.mv), first);
						return RewriteNow(f);
					}
				}
				_MARKER_ last = o.GetMarker();
				AppendReplace(last, last, ma);
				AppendSection(_MEMST_(c.mv), last);
				return RewriteNow(f);
			}
		}
		return false;
	}
	// 
	bool OpenWrite()
	{
		if (f.Open((LPCSTR)fo.pData, false))
			return true;
		return false;
	}

public:
	// 
	bool Open(LPCSTR psz)
	{
		if (Malloc_f(fo, psz)) {
			if (f.Open(psz, true))
				return true;
			if (f.Open(psz, false))
				return true;
		}
		return false;
	}
	// 
	void Close()
	{
		f.Close();
	}
	// 
	bool QueryProfileString(LPCSTR pszAppName, LPCSTR pszKeyName, LPSTR pBuff, size_t nBuffSize)
	{
		_COMMAND_ c;

		c.commandType = commandQueryKey;
		c.ma.pData = (BYTE *)const_cast<LPSTR>(pszAppName);
		c.ma.nDataLen = strlen(pszAppName);
		c.mk.pData = (BYTE *)const_cast<LPSTR>(pszKeyName);
		c.mk.nDataLen = strlen(pszKeyName);
		c.mr.pData = (BYTE *)pBuff;
		c.mr.iDataPos = 0;
		c.mr.nDataLen = nBuffSize;

		return Command(c);
	}
	// 
	bool QueryProfileSection(LPCSTR pszAppName, LPSTR pBuff, size_t nBuffSize)
	{
		_COMMAND_ c;

		c.commandType = commandQuerySection;
		c.ma.pData = (BYTE *)const_cast<LPSTR>(pszAppName);
		c.ma.nDataLen = strlen(pszAppName);
		c.mr.pData = (BYTE *)pBuff;
		c.mr.iDataPos = 0;
		c.mr.nDataLen = nBuffSize;

		return Command(c);
	}
	// 
	bool QueryProfileSectionNames(LPSTR pBuff, size_t nBuffSize)
	{
		_COMMAND_ c;

		c.commandType = commandQuerySectionNames;
		c.mr.pData = (BYTE *)pBuff;
		c.mr.iDataPos = 0;
		c.mr.nDataLen = nBuffSize;

		return Command(c);
	}
	// 
	bool ReplaceProfileString(LPCSTR pszAppName, LPCSTR pszKey, LPCSTR pszString)
	{
		_COMMAND_ c;

		c.commandType = commandReplaceKey;
		c.ma.pData = (BYTE *)const_cast<LPSTR>(pszAppName);
		c.ma.nDataLen = strlen(pszAppName);
		c.mk.pData = (BYTE *)const_cast<LPSTR>(pszKey);
		c.mk.nDataLen = strlen(pszKey);
		c.mv.pData = (BYTE *)const_cast<LPSTR>(pszString);
		c.mv.nDataLen = strlen(pszString);

		return Command(c);
	}
	// 
	bool ReplaceProfileSection(LPCSTR pszAppName, LPCSTR pszString)
	{
		_COMMAND_ c;

		c.commandType = commandReplaceSection;
		c.ma.pData = (BYTE *)const_cast<LPSTR>(pszAppName);
		c.ma.nDataLen = strlen(pszAppName);
		c.mv.pData = (BYTE *)const_cast<LPSTR>(pszString);
		c.mv.nDataLen = StrLen0(pszString);

		return Command(c);
	}
	// 
	static bool ReplaceProfileSection(LPCSTR pszAppName, LPCSTR pszString, LPCSTR pszFileName)
	{
		CProfStr a;
		if (a.Open(pszFileName)) {
			if (a.ReplaceProfileSection(pszAppName, pszString))
				return true;
		}
		return false;
	}
};
