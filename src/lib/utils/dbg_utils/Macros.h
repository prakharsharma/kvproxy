#ifndef _MACROS_H_
#define _MACROS_H_

#define TRACE(stream, msg) \
    (stream)<<"["<<__FUNCTION__<<"@"<<__FILE__":"<<__LINE__<<"]"<<msg<<std::endl

#endif
