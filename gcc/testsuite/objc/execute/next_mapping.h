/* This file "renames" various ObjC runtime entry points
   (and fakes the existence of several others)
   if the NeXT runtime is being used.  */
/* Author: Ziemowit Laski <zlaski@apple.com>  */

#ifdef __NEXT_RUNTIME__
#include <objc/objc-class.h>

#define objc_get_class(C)			objc_getClass(C)
#define objc_get_meta_class(C)			objc_getMetaClass(C)
#define class_get_class_method(C, S)		class_getClassMethod(C, S)
#define class_get_instance_method(C, S)		class_getInstanceMethod(C, S)
#define method_get_imp(M)			(((Method)M)->method_imp)
#define sel_get_name(S)				sel_getName(S)
#define class_create_instance(C)		class_createInstance(C, 0)
#define	class_get_class_name(C)			object_getClassName(C)
#define class_get_super_class(C)		(((struct objc_class *)C)->super_class)
#define object_get_super_class(O)		class_get_super_class(*(struct objc_class **)O)
#define objc_lookup_class(N)			objc_lookUpClass(N)
#define object_get_class(O)			(*(struct objc_class **)O)
#define class_is_class(C)			(CLS_GETINFO((struct objc_class *)C, CLS_CLASS)? YES: NO)
#define class_is_meta_class(C)			(CLS_GETINFO((struct objc_class *)C, CLS_META)? YES: NO)
#define object_is_class(O)			class_is_meta_class(*(struct objc_class **)O)
#define object_is_meta_class(O)			(class_is_meta_class(O) && class_is_meta_class(*(struct objc_class **)O))

/* You need either an empty +initialize method or an empty -forward:: method. 
   The NeXT runtime unconditionally sends +initialize to classes when they are 
   first used, and unconditionally tries to forward methods that the class 
   doesn't understand (including +initialize). If you have neither +initialize 
   nor -forward::, the runtime complains.  

   The simplest workaround is to add

      + initialize { return self; }

   to every root class @implementation.  */

#endif  /* #ifdef __NEXT_RUNTIME__ */
