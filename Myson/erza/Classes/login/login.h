#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class LoginScene : public cocos2d::CCLayer, public EditBoxDelegate
{
public:

	virtual bool init();
	
	static cocos2d::CCScene* scene();
	CREATE_FUNC(LoginScene);

	//当编辑框获得焦点时将被调用  
	virtual void editBoxEditingDidBegin(EditBox* editBox);
	//当编辑框失去焦点后将被调用 
	virtual void editBoxEditingDidEnd(EditBox* editBox);
	//当编辑框内容发生改变将被调用  
	virtual void editBoxTextChanged(EditBox* editBox, const std::string& text);
	//当编辑框的结束操作被调用
	virtual void editBoxReturn(EditBox* editBox);

private:
	EditBox * editBox;
};



#endif 