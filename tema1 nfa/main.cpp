/*
#include <iostream>
#include <cstring>
using namespace std;
char lit,cuv[101],stiva[101];
int x,y,t,nrstari,nrmuchii,stareinitiala,nrstarifinale,i,a[101][101][101],sf[101],starecurenta,ok;

void nfa(int starecurenta,char cuv[],int k)
{
    char prima=cuv[0];
    if(strlen(cuv)==0 and sf[starecurenta]==1)
    {
        ok=1;
        cout<<"Cuvantul este acceptat";
        return;
    }
    if(strlen(cuv)!=0 and ok==0)
    {
        for(int i=0; i<nrstari; i++)
            if(a[starecurenta][i][prima-'a']==1)
            {
                stiva[k]=prima;
                strcpy(cuv,cuv+1);
                nfa(i,cuv,k+1);
                for(int j=strlen(cuv); j>0; j--)
                    cuv[j]=cuv[j-1];
                cuv[0]=prima;
            }
    }
}
int main()
{
    cin>>nrstari>>stareinitiala>>nrstarifinale;
    for(i=0; i<nrstarifinale; i++)
    {
        cin>>t;
        sf[t]=1;
    }
    cin>>nrmuchii;
    for(i=0; i<nrmuchii; i++)
    {
        cin>>x>>y;
        cin>>lit;
        a[x][y][lit-'a']=1;
    }
    cin>>cuv;
    nfa(stareinitiala,cuv,0);
    if(ok==0)
        cout<<"Cuvantul nu este acceptat";
    return 0;
}
*/
#include <iostream>
#include<bits/stdc++.h>
using namespace std;

struct Nod
{
    int info,grad;
    Nod *fiu,*tata,*frate;
};
Nod* newNod(int val) ///initializare nod nou
{
    Nod *node=new Nod;
    node->info=val;
    node->grad=0;
    node->fiu=NULL;
    node->tata=NULL;
    node->frate=NULL;
    return node;
}
Nod* mergebt(Nod *a,Nod*b) /// facem arborele care are info mai mare fiul celuilalt
{
    if(a->info>b->info)
        swap(a,b);
    b->tata=a;
    b->frate=a->fiu;
    a->fiu=b;
    a->grad++;
    return a;
}
list<Nod*> unionbh(list<Nod*> v1,list<Nod*> v2) ///union-ul a 2 heap-uri
{
    list<Nod*> lista; ///va contine rezultatul unionului
    list<Nod*>::iterator i=v1.begin();/// luam i inceputul primului heap
    list<Nod*>::iterator j=v2.begin();/// luam j inceputul celui de-al doilea heap
    while(i!=v1.end() and j!=v2.end())/// cat timp nu ajunge la finalul heap-ului
    {
        if((*i)->grad<=(*j)->grad) ///pune in rezultat pe cel cu info mai mica
        {
            lista.push_back(*i);
            i++;
        }
        else
        {
            lista.push_back(*j);
            j++;
        }
    }
    while(i!=v1.end()) ///in cazul in care raman elemente in primul heap
    {
        lista.push_back(*i);
        i++;
    }
    while(j!=v2.end()) ///in cazul in care raman elemente in cel de-al doilea heap
    {
        lista.push_back(*j);
        j++;
    }
    return lista;
}
list<Nod*> refacere(list<Nod*> heap) ///reface heap-ul astfel incat sa nu exite 2 arbori cu acelasi grad si sa fie in ordine crescatoare
{
    if(heap.size()<=1) ///daca heap-ul este nul sau are un arbore cu gradul 0 il returnam
        return heap;
    list<Nod*> aheap;
    list<Nod*>::iterator i,j,k;
    i=j=k=heap.begin();
    if(heap.size()==2) ///daca e format din 2 arbori crestem iteratorii pt respectivii
    {
        j=i;
        j++;
        k=heap.end();
    }
    else    ///daca nu retinem urmatorii 2
    {
        j++;
        k=j;
        k++;
    }
    while(i!=heap.end()) ///parcurgem heap-ul
    {
        if(j==heap.end()) ///daca un singur element trebuie verificat, crestem iteratorul
            i++;
        else if((*i)->grad<(*j)->grad) /// daca ordinea este in regula, trecem mai departe
        {
            i++;
            j++;
            if(k!=heap.end()) ///daca k nu ajunge la final, il crestem
                k++;
        }
        else if(k!=heap.end() and (*i)->grad==(*j)->grad and (*i)->grad==(*k)->grad) ///daca 3 arbori au acelasi grad si al treilea nu este ultimul
        {
            ///creste iteratorii, pt ca merge-ul intre ultimii 2 se va face ulterior
            i++;
            j++;
            k++;
        }
        else if((*i)->grad==(*j)->grad) ///daca 2 arbori au acelasi grad, face merge-ul dintre ei, iar iteratorul pt al doilea se sterge
        {
            Nod * node;
            *i=mergebt(*i,*j);
            j=heap.erase(j);
            if(k!=heap.end()) ///daca al 3-lea nu este ultimul, crestem iteratorul
                k++;
        }
    }
    return heap;
}
list<Nod*> insertarbore(list<Nod*> heap, Nod* arbore) ///adaugarea unui arbore in heap
{
    list<Nod*> lista; ///heap temporar
    lista.push_back(arbore);///adaugam arborele in heap
    lista=unionbh(heap,lista);///facem union-ul
    return refacere(lista);///si returnam heap ul rearanjat
}
list<Nod*> insert(list<Nod*> heap, int val) ///inserarea unei valori in heap
{
    Nod*nod=newNod(val);///cream un nou nod care sa aiba informatia val, practic un arbore de grad 0
    return insertarbore(heap,nod);///pe care il inseram in heap
}
Nod* getmin(list<Nod*> heap) ///intoarce minimul
{
    list<Nod*>::iterator i=heap.begin(); ///luam un iterator pt a cauta minimul de la inceput la final in lista radacinilor
    Nod* minim=*i;///il initializam cu prima radacina
    while(i!=heap.end())
    {
        if((*i)->info<minim->info)///daca gasim una care informatia mai mica il modificam
            minim=*i;
        i++;
    }
    return minim;
}
list<Nod*> deleteminarbore( Nod* arb)///stergem minimul dintr-un arbore
{
    list<Nod*> heap;///retinem noul heap
    Nod* n=arb->fiu;///pornim de la fiul sau si inseram elementele in noul heap
    Nod* x;
    while(n)
    {
        x=n;
        n=n->frate;
        x->frate=NULL;
        heap.push_front(x);
    }
    return heap;
}
list<Nod*> deleteminim(list<Nod*> heap)///stergem minimul din heap
{
    list<Nod*> aheap,bheap;
    Nod* minim;
    minim=getmin(heap);
    list<Nod*>::iterator i;
    i=heap.begin();///parcurgem lista de radacini
    while(i!=heap.end())
    {
        if(*i!=minim)///daca arborele nu contine minimul, il inseram in noul heap
            aheap.push_back(*i);
        i++;
    }
    bheap=deleteminarbore(minim);///stergem minimul din arborele ce contine minimul
    aheap=unionbh(aheap,bheap);///combinam cele 2 heap-uri rezultate:cel format din arborii care nu contin minimul si cel format cu ce a ramas dupa eliminarea minimului
    aheap=refacere(aheap);
    return aheap;
}
void afisarearbore(Nod*node) ///afisarea unui arbore
{
    while(node)
    {
        cout<<node->info<<" "; ///afisam info din nodul curent
        afisarearbore(node->fiu); ///apoi din nodul fiu
        node=node->frate; ///iar ulterior din cel frate/vecin
    }
}
void afisareheap(list<Nod*> heap) ///afisam heap-ul
{
    cout<<"Continutul heap-ului este : "<<'\n';
    list<Nod*>::iterator i; ///luam un iterator pt a parcurge lista de radacini
    i=heap.begin();
    while(i!=heap.end())
    {
        afisarearbore(*i);///pt fiecare radacina afisam arborele la care pointeaza iteratorul
        cout<<endl;
        i++;
    }
}
void buildheap(list<Nod*> &heap,int &n)
{
    int x;
    for(int i=0;i<n;i++)
    {
        cin>>x;
        heap=insert(heap,x);
    }
}
list<Nod*> merge2bh(list<Nod*> a1,list<Nod*> a2)
{
    list<Nod*> b;
    b=unionbh(a1,a2);
    return refacere(b);
}
int n,x,y;
int main()
{
    /*cin>>n;
    list<Nod*> Heap;
    for (int i=1;i<=n;i++)
    {
        cin>>x;
        if(x==1)
        {
            cin>>y;
            Heap=insert(Heap,y);
        }

    }*/
    list<Nod*> aheap;
    aheap=insert(aheap,10);
    aheap=insert(aheap,20);
    aheap=insert(aheap,30);
    aheap=insert(aheap,51);
    aheap=insert(aheap,42);
    aheap=insert(aheap,16);
    aheap=insert(aheap,5);
    aheap=insert(aheap,131);
    aheap=insert(aheap,39);
    aheap=insert(aheap,81);
    aheap=insert(aheap,26);
    aheap=insert(aheap,1);
    aheap=insert(aheap,43);
    list<Nod*> bheap;
    bheap=insert(bheap,79);
    bheap=insert(bheap,55);
    bheap=insert(bheap,11);
    bheap=insert(bheap,51);
    bheap=insert(bheap,21);
    bheap=insert(bheap,17);
    afisareheap(aheap);
    afisareheap(bheap);
    Nod* node=getmin(aheap);
    cout<<node->info<<'\n';
    Nod* node1=getmin(bheap);
    cout<<node1->info<<'\n';
    list<Nod*>x;
    x=unionbh(aheap,bheap);
    x=refacere(x);
    afisareheap(x);
    x=deleteminim(x);
    afisareheap(x);
    x=deleteminim(x);
    afisareheap(x);
    afisareheap(bheap);
    list<Nod*> cheap;
    int l;
    cin>>l;
    buildheap(cheap,l);
    afisareheap(cheap);
    return 0;
}
