#ifndef __HH_CONFIG_H_
#define __HH_CONFIG_H_

#define __STL_TEMPLATE_NULL template <>
#define __STL_MEMBER_TEMPLATES
#define __STL_CLASS_PARTIAL_SPECIALIZATION
#define __STL_NULL_TMPL_ARGS <>
#define __STL_TRY try
#define __STL_UNWIND(action) catch(...){action;throw;}

#endif
