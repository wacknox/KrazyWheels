#include "KrazyXMLNode.h"
//..,
class KrazyTextXML : public KrazyXMLNode 
{
public :
	KrazyTextXML();
	virtual ~KrazyTextXML();
//..,
	void loadText(string _s_xml);
	KrazyTextXML* getParent();	
};