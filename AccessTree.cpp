#include "AccessTree.h"

vector<ATRPATHLCM>* AccessTree::GetATRPATHLCM(vector<string>* atrs,BIGNUM *p)
{
	int threshold ,track;
    stack<Node> *ps;
    deque<Node> *pNodeQue;
    vector<ATRPATHLCM> *storeLCM;
  
    pNodeQue = new deque<Node>();
    storeLCM = new vector<ATRPATHLCM>();

    if(!IsSatisfied(atrs,pNodeQue))
    {
		return storeLCM;
    }

    ps =  new stack<Node>();

    while(pNodeQue->size() != 0)
    {
    	Node node = pNodeQue->front();
       	if(!node.IsLeaf())
       	{
	  		threshold =  atoi(node.GetValue().c_str());
          	track = ps->size() - threshold + 1;
          	UpdateATRPATHLCM(storeLCM,
				ps,
				threshold,
				track,
				p);
       }
       ps->push(node);
       pNodeQue->pop_front();
   }
	
   return storeLCM;

}
void AccessTree::UpdateATRPATHLCM(vector<ATRPATHLCM>* pv,
				      			stack<Node>* ps,
				     			int threshold,
							int track,
				      			BIGNUM* p)
{
	vector<int>* partyIndex = new vector<int>();

    vector<string> *atrs =  new vector<string>();

    GetPartyIndex(ps,partyIndex,threshold);
    vector<ATRPATHLCM>::iterator iter;  
    Polynomial polynomial = Polynomial();
    polynomial.SetP(p);
    for(int i = 0;i<threshold;i++)
    {
    	Node childNode = ps->top();

		BIGNUM *lc = polynomial.GetLagIntCoe(partyIndex,childNode.GetPosition());

        if(childNode.IsLeaf())
        {
	        ATRPATHLCM atrLCM = ATRPATHLCM(childNode.GetValue(),p);
	        atrLCM.MulLagCoe(lc);
            pv->push_back(atrLCM);
			atrs->push_back(childNode.GetValue());
        }
        else
        {
        	for (iter=pv->begin();iter!=pv->end();iter++)  
            {  
            	if(iter->GetTrack() == ps->size())
	            {
		       		iter->MulLagCoe(lc);
		       		atrs->push_back(iter->GetName());
	            }	   
            }  
        }
        ps->pop();
    }
 
    for (iter=pv->begin();iter!=pv->end();iter++)  
    {  
    	if(contain(atrs,iter->GetName()))
		{
			iter->SetTrack(track); 
		}	   
    }  
       
}

void AccessTree::GetPartyIndex(stack<Node> *ps,vector<int> * pv,int threshold)
{
    for(int i=0;i<threshold;i++)
    {
        int position =(&ps->top()-i)->GetPosition();
        pv->push_back(position); 
    }
}

AccessTree::AccessTree(string accessTree)
{
   	this->accessTree = accessTree;
}
AccessTree::AccessTree(TiXmlElement *element)
{
   	TiXmlDocument * doc = element_wrap(element);
   	accessTree = xml2string(doc);

}
bool AccessTree::IsSatisfied(vector<string> *atrs,deque<Node> *pNodeQue)
{
   	pNodeQue->clear();
   	TiXmlElement * root = string2xml(accessTree)->RootElement();
   	Node node = Node(root);
   
   	bool isSatisfied = SatisfyAceessTree(atrs,root,pNodeQue);

   	if(isSatisfied)
   	{
		pNodeQue->push_back(node);
   	}
   	else
   	{
		pNodeQue->clear();
   	}

   	return isSatisfied; 
}

bool AccessTree::SatisfyAceessTree(vector<string> *atrs,TiXmlElement *element,deque<Node> *pNodeQue)
{
	int count = 0;
   	int orginalSize;
   	int threshold;
   	string value ;
   	Node parentNode;
 
  	 parentNode = Node(element);
  	 value = parentNode.GetValue();
   	if(parentNode.IsLeaf())
   	{
    	return  contain(atrs,value);
   	}

   	threshold = atoi(value.c_str());
   	TiXmlElement * child = element->FirstChildElement();

  	while(child && count < threshold)
   	{
    	orginalSize = pNodeQue->size();
    
      	if(SatisfyAceessTree(atrs,child,pNodeQue))
      	{
	 		Node childNode = Node(child);
         	pNodeQue->push_back(childNode);
	 		count++;
      	}
      	else
      	{
        	while(pNodeQue->size() != orginalSize)
         	{
           		pNodeQue->pop_back();
         	}
      	}
    
     	 child = child->NextSiblingElement();
   	}
  	return count >= threshold ? true : false ;
}
