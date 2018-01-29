/*
 * Copyright 2016- huaxz <huaxz1986@163.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: huaxz1986@163.com (huaxz)
 */
#ifndef BINARYTREENODE
#define BINARYTREENODE
#include<memory>
#include<string>
#include<sstream>
namespace IntroductionToAlgorithm {
namespace TreeAlgorithm {
//!BinaryTreeNode：二叉树的节点，算法导论xx章xx节
/*!
*
* 任何一个节点都有两个强引用指向左右子节点，以及一个弱引用指向它的父节点。节点还有一个`key`成员包含具体的数据
*/
template<typename KType> struct BinaryTreeNode
{
public:
    typedef KType KeyType;  /*!< 节点保存的数据的类型*/
    //! 默认构造函数
    /*!
     * 所有的成员变量都采取默认值
     */
    BinaryTreeNode()                                        //隐式转换：可以用单个实参来调用的构造函数定义了从形参类型到该类型的隐式转换
        :parent(std::weak_ptr<BinaryTreeNode>()),           //弱引用指向父结点
        lchild(std::shared_ptr<BinaryTreeNode>()),          //强引用指向左孩子
        rchild(std::shared_ptr<BinaryTreeNode>()),          //强引用指向右孩子                              
        key(KeyType())                                      //具体的键值
    {}
    //! 显式构造函数
    /*!
     *\param keyvalue:节点要保存的数据内容
     *
     * 指定`key`成员需要赋值的数据
     */
    explicit BinaryTreeNode(const KeyType& keyvalue)        //显示构造函数
        :parent(std::weak_ptr<BinaryTreeNode>()),           //explicit用来防止由构造函数定义的隐式转换
        lchild(std::shared_ptr<BinaryTreeNode>()),          //explicit只能用于类内部的构造函数声明上，不能用在类外部的函数定义上
        rchild(std::shared_ptr<BinaryTreeNode>()),          //explicit可以避免不合时宜的类型转变
        key(keyvalue)                                       //explicit禁止编译器执行非预期的类型转换
    {}

    //! to_string:返回该节点的字符串描述
    /*!
     * \return : 本节点的描述字符串
     *
     * 该函数打印本节点的`key`，以及父节点（若存在）、子节点（若存在）的`key`
     */
    virtual std::string to_string(){
        std::ostringstream os;
        os<<"node:"<<key;
        if(auto shared_p=parent.lock())                     //把弱引用转换为强引用
            os<<"\n\t parent:"<<shared_p->key;
        else os<<"\n\t parent:"<<"null_ptr";

        os<<"\n\t left_child:";
        if(lchild)  os<<lchild->key;
        else   os<<"nullptr";

        os<<"\n\t right_child:";
        if(rchild) os<<rchild->key;
        else os<<"nullptr";
        return os.str();
    }
    //! to_xml:返回以该节点为根的子树的`xml`描述。
    /*!
     * \return : 本节点子树的`xml`描述的字符串
     *
     * 该函数返回以本节点为根的子树的`xml`描述。对子节点递归调用从而生成`xml`数据
     */
    virtual std::string to_xml(){                         //转换为网页显示  
        std::ostringstream os;
        os<<"\n<node>"<<key<<"\n";
        if(auto shared_p=parent.lock())
        {
            os<<"\t <parent>"<<shared_p->key<<"</parent>";
        }else
        {
            os<<"\t <parent>"<<"null_ptr"<<"</parent>";
        }
        os<<"\n\t <left_child>"<<(lchild? lchild->to_xml(): "nullptr")<<"</left_child>"\
          <<"\n\t<right_child>"<<(rchild? rchild->to_xml():"nullptr")<<"</right_child>"\
          <<"\n</node>";
        return os.str();
    }
    //! is_left_child:判断本节点是否左子节点。
    /*!
     *\return 返回`true`或者`false`
     * 该函数判断本节点是否是左子节点。如果本节点的父节点为空，则返回`false`；如果本节点的父节点非空且本节点是父节点的左子节点，则返回`true`；否则返回`false`
     */
    bool is_left_child()                                //判断是否是左孩子
    {
       if (auto shared_p = parent.lock())
       {
            if (this==shared_p->lchild.get())
                return true;
       }
       return false;
    }
    //! is_right_child:判断本节点是否右子节点。
    /*!
     *\return 返回`true`或者`false`
     * 该函数判断本节点是否是右子节点。如果本节点的父节点为空，则返回`false`；如果本节点的父节点非空且本节点是父节点的右子节点，则返回`true`；否则返回`false`
     */
    bool is_right_child()
    {
       if (auto shared_p = parent.lock())
       {
            if (this==shared_p->rchild.get())
                return true;
       }
       return false;
    }
    std::weak_ptr<BinaryTreeNode> parent;   /*!< 节点的父节点的弱引用*/
    std::shared_ptr<BinaryTreeNode> lchild; /*!< 节点的左子节点的强引用*/
    std::shared_ptr<BinaryTreeNode> rchild; /*!< 节点的右子节点的强引用*/
    KeyType key;                                 /*!< 节点的保存的数据*/
};
}
}

#endif // BINARYTREENODE

