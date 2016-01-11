/****************************************************************************
 Copyright (C) 2013 Henry van Merode. All rights reserved.
 Copyright (c) 2015 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __CC_PU_SCRIPT_COMPILER_H__
#define __CC_PU_SCRIPT_COMPILER_H__
#include "base/CCRef.h"
#include "extensions/Particle3D/PU/CCPUScriptParser.h"

NS_CC_BEGIN

/**
* @addtogroup _3d
* @{
*/

/** This enum holds the types of the possible abstract nodes */
enum PUAbstractNodeType
{
    ANT_UNKNOWN,
    ANT_ATOM,
    ANT_OBJECT,
    ANT_PROPERTY,
    ANT_IMPORT,
    ANT_VARIABLE_SET,
    ANT_VARIABLE_ACCESS
};
class CC_DLL PUAbstractNode;
typedef std::list<PUAbstractNode*> PUAbstractNodeList;

/** @class PUAbstractNode
*@brief @~english Internal class, The Abstract class of script element.
@~chinese 内部类，脚本中元素的抽象类。
*/
class CC_DLL PUAbstractNode
{
public:
    std::string file;
    unsigned int line;
    PUAbstractNodeType type;
    PUAbstractNode *parent;
    Ref *context;
    
public:
    PUAbstractNode(PUAbstractNode *ptr):line(0), type(ANT_UNKNOWN), parent(ptr), context(nullptr)
    {


    }
    virtual ~PUAbstractNode(){}
    /** Returns a new AbstractNode which is a replica of this one. */
    virtual PUAbstractNode *clone() const = 0;
    /** Returns a string value depending on the type of the AbstractNode.  */
    virtual std::string getValue() const = 0;
};


/** @class PUObjectAbstractNode
*@brief @~english Internal class, This specific abstract node represents a script object.
@~chinese 内部类，脚本中对象的抽象类。
*/
class CC_DLL PUObjectAbstractNode : public PUAbstractNode
{
private:
    std::map<std::string,std::string> _env;
public:
    std::string name, cls;
    std::vector<std::string> bases;
    unsigned int id;
    bool abstract;
    PUAbstractNodeList children;
    PUAbstractNodeList values;
    PUAbstractNodeList overrides; 
public:
    PUObjectAbstractNode(PUAbstractNode *ptr);
    virtual ~PUObjectAbstractNode();
    PUAbstractNode *clone() const;
    std::string getValue() const;
    
    
    void addVariable(const std::string &name);
    void setVariable(const std::string &name, const std::string &value);
    std::pair<bool,std::string> getVariable(const std::string &name) const;
    const std::map<std::string,std::string> &getVariables() const;
};

/** @class PUPropertyAbstractNode
*@brief @~english Internal class, This abstract node represents a script property.
@~chinese 内部类，脚本中属性的抽象类。
*/
class CC_DLL PUPropertyAbstractNode : public PUAbstractNode
{
public:
    std::string name;
    unsigned int id;
    PUAbstractNodeList values;
public:
    PUPropertyAbstractNode(PUAbstractNode *ptr);
    virtual ~PUPropertyAbstractNode();
    PUAbstractNode *clone() const;
    std::string getValue() const;
};

/** @class PUObjectAbstractNode
*@brief @~english Internal class, This is an abstract node which cannot be broken down further.
@~chinese 内部类，脚本中原子类型（不可再细分）的抽象类。
*/
class CC_DLL PUAtomAbstractNode : public PUAbstractNode
{
public:
    std::string value;
    unsigned int id;
public:
    PUAtomAbstractNode(PUAbstractNode *ptr);
    PUAbstractNode *clone() const;
    std::string getValue() const;
private:
    void parseNumber() const;
};

class CC_DLL PUParticleSystem3D;

/** @class PUScriptCompiler
*@brief @~english Internal class, The compiler of scrip.
@~chinese 内部类，脚本编译器。
*/
class CC_DLL PUScriptCompiler
{

private:
    bool compile(const PUConcreteNodeList &nodes, const std::string &file);
    bool isNameExcluded(const std::string &cls, PUAbstractNode *parent);
    
public:
    typedef std::map<std::string,unsigned int> IdMap;
    
    static PUScriptCompiler* Instance();

    /**
    * @~english Set the particle system.
    * @~chinese 设置粒子系统
    * @param pu      @~english The particle system. @~chinese 粒子系统。
    */
    void setParticleSystem3D(PUParticleSystem3D *pu);

    /**
    * @~english Compile a file.
    * @~chinese 编译一个文件。
    * @param file      @~english The path of file. @~chinese 文件路径。
    * @param isFirstCompile      @~english Is first compile this file. @~chinese 是否第一次编译该文件。
    */
    const PUAbstractNodeList* compile(const std::string &file, bool &isFirstCompile);
    
    /**
    * @~english PUConcreteNodeList Convert to PUAbstractNodeList.
    * @~chinese PUConcreteNodeList转换为PUAbstractNodeList。
    * @param nodes      @~english The PUConcreteNodeList. @~chinese PUConcreteNodeList。
    * @param aNodes      @~english The PUAbstractNodeList. @~chinese PUAbstractNodeList。
    */
    void convertToAST(const PUConcreteNodeList &nodes,PUAbstractNodeList &aNodes);
    
    std::map<std::string,std::string> env;
    
private:
    PUScriptCompiler();
    virtual ~PUScriptCompiler();

    void visitList(const PUConcreteNodeList &nodes);
    void visit(PUConcreteNode *node);
private:
    
    std::map<std::string, PUAbstractNodeList> _compiledScripts;
    PUAbstractNode *_current;
    PUAbstractNodeList *_nodes;
    PUParticleSystem3D *_PUParticleSystem3D;
};

// end of _3d group
/// @}

NS_CC_END

#endif /* defined(__ssaafsdf__CCScriptCompile__) */
