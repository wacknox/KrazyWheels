#include "StdAfx.h"
#include "KrazyXMLNode.h"
#include "KrazyNodeTraverserIndexers.h"
///////////////////////////////////////////////////////////////
#define _START_NODE(tag,node)			tag.append("<");tag.append(node)
#define _MAKE_ATTR(tag , name , value)	tag.append(" ");tag.append(name);tag.append("=\"");tag.append(value);tag.append("\"")
#define _END_NODE(tag)					tag.append(">")
#define _END_NODE_DATA(tag)				tag.append("/>")
#define _END_START_NODE(tag , val)		tag.append("</");tag.append(val);tag.append(">")
KrazyXMLNode* KrazyXMLNode::_PTRMe = 0;
KrazyXMLNode* KrazyXMLNode::getXMLNodeParent()
{
	if(_PTRMe == 0){
		_PTRMe = new KrazyXMLNode;
	}
	return _PTRMe;
}
/////////////////////////////////////////
KrazyXMLAttribute*  KrazyXMLNode::get_AttributeByTypeIfAny(string type)
{
	int attrib_count = _Attributes.size();
	for(int i = 0; i < attrib_count; i++){
		KrazyXMLAttribute* attrib = _Attributes[i];
		string name = attrib->get_Name() ;
		if(!name.compare(type))
			return attrib;
	}
	return 0;
}
////////////////////////////////////////
string KrazyXMLNode::get_XML()
{
	_XML.clear();
	KrazyNodeDepthIndexerMap _map_to_creat;
	KrazyXMLNode* ptr_CurrentParent = this;
	while(ptr_CurrentParent){
		if(_map_to_creat.find(ptr_CurrentParent)){
			_helper_indexer* ptr_indexer = _map_to_creat[ptr_CurrentParent];
			string tag  =  ptr_CurrentParent->get_NodeName();
			_START_NODE(_XML , tag);
			int count_attrib = ptr_CurrentParent->get_Attributes_Count();
			for(int i =  0; i < count_attrib; i++){
				KrazyXMLAttribute* ptr_attrib = ptr_CurrentParent->get_Attribute(i);
				string res , name , value;
				ptr_attrib->get_Name_Data(name,value);
				_MAKE_ATTR(_XML  , name , value);
			}
			ptr_indexer->_Size = ptr_CurrentParent->get_Kids_Count();
			if(ptr_indexer->_Size == 0){
				_END_NODE_DATA(_XML);
				ptr_CurrentParent = ptr_CurrentParent->get_PTRParent();
				continue;
			}
			_END_NODE(_XML);
			ptr_CurrentParent = ptr_CurrentParent->get_KidsNode(ptr_indexer->_Cursor++);
			continue;
		}
		// simulating recursion for whatever current parent is in existance..,
		_helper_indexer* indexer = _map_to_creat[ptr_CurrentParent];
		if((indexer->_Size > 0 && indexer->_Cursor < indexer->_Size)){
			ptr_CurrentParent = ptr_CurrentParent->get_KidsNode(indexer->_Cursor++);
		}
		else  if((indexer->_Size > 0 && indexer->_Cursor == indexer->_Size)){
			_END_START_NODE(_XML,ptr_CurrentParent->get_NodeName());
			ptr_CurrentParent = ptr_CurrentParent->get_PTRParent();
		}
	}

	return _XML;
}
////////////////////////////////////////////////////
void KrazyXMLNode::load_XML(string xml)
{
	_XML.clear();
	_XML = xml;
	parse();
}
///////////////////////////////////////////////////
void KrazyXMLNode::parse()
{
	const char _T_OPEN('<');
	const char _T_CLOSE('>');
	const char _T_SLASH('/');
	const char _T_EQUALS('=');
	const char _T_SPACE(' ');
	const char _T_QUOTES('"');
	const char _T_QUOTE(39);
	enum ParseState
	{
       ParseState_Default,
	   ParseState_ReadNodeName,
	};
	int xml_len = _XML.size();
	KrazyXMLNode* ptr_currentParent = this;
	int indexer = 0;
	bool first_pass_tag(true);
	bool first_pass_return(true);
	while(indexer < xml_len){
		char ch = _XML.at(indexer++);
		if(ch == _T_OPEN){			
			string			tag_Name ;
			string			attribName;
			int				quotes_count = 0;
			ParseState		parse_state = ParseState_ReadNodeName;
			while( indexer < xml_len && ch != _T_CLOSE ){
				ch = _XML.at(indexer);
				if(ch == _T_SLASH &&  _XML.at(indexer - 1) == _T_OPEN){ // close..,
					ptr_currentParent = ptr_currentParent->get_PTRParent();
					while( ch != _T_CLOSE)
						ch = _XML.at(indexer++);
					break;
				}
				else if(ch == _T_SLASH &&  _XML.at(indexer + 1) == _T_CLOSE){
					if(ptr_currentParent->get_NodeName().empty()){
						ptr_currentParent->set_NodeName(tag_Name);
						tag_Name.clear();
					}
					ptr_currentParent = ptr_currentParent->get_PTRParent();
					break;
				}
				else if(ch == _T_CLOSE){
					if(ptr_currentParent->get_NodeName().empty()){
						ptr_currentParent->set_NodeName(tag_Name);
						tag_Name.clear();
					}
					continue;
				}
				else if(ch == _T_EQUALS){
					attribName = tag_Name;
					tag_Name.clear();
					indexer++;
					continue;
				}
				else if(ch == _T_QUOTES || ch == _T_QUOTE){
				    if(++quotes_count == 2){
						quotes_count = 0;
						ptr_currentParent->getNewAttribute(attribName , tag_Name);
						attribName.clear();tag_Name.clear();
					}
					indexer++;
					continue;
				}
				else if(ch == _T_SPACE){
					switch(parse_state){
					case ParseState_ReadNodeName:
						ptr_currentParent->set_NodeName(tag_Name);tag_Name.clear();
						parse_state = ParseState_Default;
						break;
					}
					indexer++;
					continue;
				}
				else{
					if(_XML.at(indexer - 1) == _T_OPEN){
						if(first_pass_tag){
							first_pass_tag = false;
						}
						else{
							KrazyXMLNode* child_node = ptr_currentParent->getNewNode();
							ptr_currentParent = child_node;
						}
					}
				}
				tag_Name.push_back(ch);indexer++;				
			}
			if(!ptr_currentParent){
				return;
			}
		}
	}
}

