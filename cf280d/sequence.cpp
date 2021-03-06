#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;

inline void READ(int &x)
{
	char c;
	x=0;
	bool minus(false);
	do c=getchar(); while ((c<'0' || c>'9') && c!='-');
	if (c=='-') minus=true, c=getchar();
	do x=x*10+c-48, c=getchar(); while (c>='0' && c<='9');
	if (minus) x=-x;
}

const int maxn=1e5+5;

struct RANGE
{
	int l, r, v;
	RANGE() {}
	RANGE(int _l, int _r, int _v) : l(_l), r(_r), v(_v) {}
	RANGE operator-() const { return RANGE(l,r,-v); }
};

inline bool operator<(const RANGE &a, const RANGE &b)
{
	return a.v<b.v;
}

inline RANGE operator+(const RANGE &a, const RANGE &b)
{
	return RANGE(min(a.l,b.l),max(a.r,b.r),a.v+b.v);
}

struct NODE
{
	RANGE max, lmax, rmax, min, lmin, rmin, full;
	bool flip;
	
	void assign(int l, int r, int v)
	{
		max=lmax=rmax=min=lmin=rmin=full=RANGE(l,r,v);
	}
	
	void __flip()
	{
		*this=(NODE){-min,-lmin,-rmin,-max,-lmax,-rmax,-full,flip^1};
	}
	
	void push(NODE &son1, NODE &son2)
	{
		flip=false;
		son1.__flip(), son2.__flip();
	}
	
	void pull(const NODE &son1, const NODE &son2)
	{
		max=std::max(std::max(son1.max,son2.max),son1.rmax+son2.lmax);
		min=std::min(std::min(son1.min,son2.min),son1.rmin+son2.lmin);
		lmax=std::max(son1.lmax,son1.full+son2.lmax);
		lmin=std::min(son1.lmin,son1.full+son2.lmin);
		rmax=std::max(son2.rmax,son2.full+son1.rmax);
		rmin=std::min(son2.rmin,son2.full+son1.rmin);
		full=son1.full+son2.full;
	}
};

int N, M;
NODE f[maxn<<1];

#define pos(l,r) (((l)+(r))|((l)!=(r)))

void Ins(int l, int r, int q, int v)
{
	int mid((l+r)>>1);
	NODE &now=f[pos(l,r)], &son1=f[pos(l,mid)], &son2=f[pos(mid+1,r)];
	if (l==r)
	{
		now.assign(l,r,v);
		return;
	}
	if (now.flip) now.push(son1,son2);
	if (q<=mid) Ins(l,mid,q,v); else Ins(mid+1,r,q,v);
	now.pull(son1,son2);
}

NODE Find(int l, int r, int ql, int qr)
{
	int mid((l+r)>>1);
	NODE &now=f[pos(l,r)], &son1=f[pos(l,mid)], &son2=f[pos(mid+1,r)];
	if (l==ql && r==qr) return now;
	if (now.flip) now.push(son1,son2);
	if (qr<=mid) return Find(l,mid,ql,qr);
	if (ql>mid) return Find(mid+1,r,ql,qr);
	NODE ret;
	ret.pull(Find(l,mid,ql,mid),Find(mid+1,r,mid+1,qr));
	return ret;
}

void Flip(int l, int r, int ql, int qr)
{
	int mid((l+r)>>1);
	NODE &now=f[pos(l,r)], &son1=f[pos(l,mid)], &son2=f[pos(mid+1,r)];
	if (l==ql && r==qr)
	{
		now.__flip();
		return;
	}
	if (now.flip) now.push(son1,son2);
	if (qr<=mid) Flip(l,mid,ql,qr); else
	if (ql>mid) Flip(mid+1,r,ql,qr); else
		Flip(l,mid,ql,mid), Flip(mid+1,r,mid+1,qr);
	now.pull(son1,son2);
}

int main()
{
	freopen("sequence.in","r",stdin);
	freopen("sequence.out","w",stdout);
	READ(N);
	for (int i=1;i<=N;i++)
	{
		int x;
		READ(x);
		Ins(1,N,i,x);
	}
	READ(M);
	while (M--)
	{
		int type;
		READ(type);
		if (!type)
		{
			int x, v;
			READ(x), READ(v);
			Ins(1,N,x,v);
		} else
		{
			int l, r, k, ans=0;
			static RANGE seq[25];
			READ(l), READ(r), READ(k);
			for (int i=1;i<=k;i++)
			{
				seq[i]=Find(1,N,l,r).max;
				if (seq[i].v<=0)
					k=i-1;
				else
					Flip(1,N,seq[i].l,seq[i].r);
			}
			for (int i=1;i<=k;i++)
			{
				ans+=seq[i].v;
				Flip(1,N,seq[i].l,seq[i].r);
			}
			printf("%d\n",ans);
		}
	}
	return 0;
}

