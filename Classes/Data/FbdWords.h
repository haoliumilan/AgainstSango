//
//  FbdWords.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-15.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__FbdWords__
#define __AgainstWar__FbdWords__

#include <iostream>

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class FbdWords {
    static FbdWords* s_pcIf;
    
public:
	//Get the instance pointer
	static FbdWords* Ins();

    void   chkLegal(std::string& str);
    
    bool   CheckLegal(std::string str);
    
private:
    //forbid constructor being called outside
	FbdWords(){};
    
	//forbid copy constructor being called outside
	FbdWords(const FbdWords& );
	
	//forbid assignment
	FbdWords& operator=(const FbdWords& );
    
    
    
    std::string& replace_all(std::string& str,const std::string& old_value,const std::string& new_value);
    
};

#endif /* defined(__AgainstWar__forbiddenWords__) */
