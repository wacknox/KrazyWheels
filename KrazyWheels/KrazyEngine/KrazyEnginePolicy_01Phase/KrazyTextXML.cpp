#include "stdafx.h"
#include "KrazyTextXML.h"
//..,
KrazyTextXML::KrazyTextXML()
{
}
//.,
KrazyTextXML::~KrazyTextXML()
{
}
//..,
void KrazyTextXML::loadText(string _s_xml)
{
	load_XML(_s_xml);
}
//..,
KrazyTextXML* KrazyTextXML::getParent()
{
	return this;
}
//..,