#include "ABC.h"


int main(int argc,char* argv[])
{
    string rule = "(human||sale||business)&&(2,woman,age:8>25&&(1,date:10>34),neu)&&profession";
    RuleBuilder ruleBuilder = RuleBuilder(rule);
 	TiXmlElement *ruleXml = ruleBuilder.RuleString2XMLElement();
 	
 	
 	cout<<xml2string(element_wrap(ruleXml))<<endl;
 	cout<<ruleBuilder.Check()<<endl;
 	
	vector<string>* atrs = new vector<string>();
	atrs->push_back("man");
	atrs->push_back("age:8=37");
	atrs->push_back("human");
	atrs->push_back("neu");
	atrs->push_back("profession");
	atrs->push_back("date:10=45");
	 
	BN_CTX *ctx = BN_CTX_new();
    EC_GROUP * group = EC_GROUP_init(ctx);
    ABC abc = ABC(group);
    ABCPK abcPK = abc.GetPK();
    ABCSK abcSK = abc.GenerateABCSK(atrs);

   
    char src[100]     = "/home/jinhz/Desktop/PBC.pdf";
    char cTarget[100] = "/home/jinhz/Desktop/CPBC.pdf";
    char pTarget[100] = "/home/jinhz/Desktop/PPBC.pdf";

    // encrypt the file -aes 128
    ABC abcEn;
    abcEn.SetPK(abcPK);
    ABCEBLOCK eblock;
    int state = abcEn.EncryptFile(src,cTarget,eblock,ruleXml);
    cout<<state<<endl;
   

    ABC abcDe;
    abcDe.SetSK(abcSK);
    
  
    //decrypt the file - aes 128
    state = abcDe.DecryptFile(cTarget,pTarget,eblock);
    cout<<state<<endl;
}
