/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      Mailbox.t.cpp
 * Project:   SOT
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/




#ifndef __SOT_MAILBOX_T_CPP
#define __SOT_MAILBOX_T_CPP

#ifdef  HAVE_LIBBOOST_THREAD

#include <sot-core/mailbox.hxx>

namespace sot {

///* --- SOT PLUGIN  --- */
//#include <sot-core/mailbox.h>
//
////#undef VP_TEMPLATE_DEBUG
////#define VP_TEMPLATE_DEBUG_MODE             0
//#include <sot-core/debug.h>

namespace dg = dynamicgraph;

/*
: public dg::Entity
{
 public:
 static const std::string CLASS_NAME;

 public:
  struct sotTimestampedObject
  {
	maal::boost::Vector obj;
	struct timeval timestamp;
  };

  virtual const std::string& getClassName( void );

  Mailbox( const std::string& name );
  ~Mailbox( void );

  void post( const maal::boost::Vector& obj );
  sotTimestampedObject& get( sotTimestampedObject& res,const int& dummy );

  maal::boost::Vector& getObject( maal::boost::Vector& res,const int& time );
  struct timeval& getTimestamp( struct timeval& res,const int& time );

  bool hasBeenUpdated( void );
};
*/

/* -------------------------------------------------------------------------- */
/* --- CONSTRUCTION --------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
template< class Object >
Mailbox<Object>::
Mailbox( const std::string& name )
  :dg::Entity(name)
  ,mainObjectMutex()
  ,mainObject()
  ,update(false)

   ,SOUT( boost::bind(&Mailbox::get,this,_1,_2),
	  dg::sotNOSIGNAL,
	  "Mailbox("+name+")::output(Object)::out" )
   ,objSOUT( boost::bind(&Mailbox::getObject,this,_1,_2),
	     SOUT,
	     "Mailbox("+name+")::output(Object)::object" )
   ,timeSOUT( boost::bind(&Mailbox::getTimestamp,this,_1,_2),
	      SOUT,
	      "Mailbox("+name+")::output(Object)::timestamp" )
{
  signalRegistration( SOUT<<objSOUT<<timeSOUT );
  SOUT.setDependencyType( dg::TimeDependency<int>::BOOL_DEPENDENT );
}

template< class Object >
Mailbox<Object>::
~Mailbox( void )
{
  boost::timed_mutex::scoped_lock lockMain( mainObjectMutex );
}

/* -------------------------------------------------------------------------- */
/* --- ACCESS --------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
template< class Object >
bool Mailbox<Object>::
hasBeenUpdated( void )
{
  boost::timed_mutex::scoped_try_lock lockMain( this->mainObjectMutex );
  
  if( lockMain.owns_lock() )
    {
      return update;
    }
  else 
    {
      return false;
    }
}


/* -------------------------------------------------------------------------- */
template< class Object >
typename Mailbox<Object>::sotTimestampedObject& Mailbox<Object>::
get( typename Mailbox<Object>::sotTimestampedObject& res,const int& dummy )
{
  boost::timed_mutex::scoped_try_lock lockMain( this->mainObjectMutex );
  
  if( lockMain.owns_lock() )
    {
      res.timestamp.tv_sec=this->mainTimeStamp.tv_sec;
      res.timestamp.tv_usec=this->mainTimeStamp.tv_usec;
      
      update = false;
      res.obj = this->mainObject;
    }

  return res;
}

/* -------------------------------------------------------------------------- */
template< class Object >
void Mailbox<Object>::
post( const Object& value )
{
  boost::timed_mutex::scoped_lock lockMain( this->mainObjectMutex );
  mainObject = value;
  gettimeofday( &this->mainTimeStamp, NULL );
  update = true;
  SOUT.setReady();

  return;
}

template< class Object >
Object& Mailbox<Object>::
getObject( Object& res,const int& time )
{ 
	const sotTimestampedObject & data = SOUT(time);
	res = data.obj; return res;
}

template< class Object >
timeval& Mailbox<Object>::
getTimestamp( struct timeval& res,const int& time )
{ 
	const sotTimestampedObject & data = SOUT(time);
	res.tv_sec = data.timestamp.tv_sec ; 
	res.tv_usec = data.timestamp.tv_usec ; 
	return res;
}

/* Macro for template specialization */
#define MAILBOX_TEMPLATE_SPE(S) \
namespace sot { \
template void Mailbox<S>::post(const S& obj ); \
template maal::boost::Vector&  Mailbox<S>::getObject( S& res,const int& time ); \
template bool Mailbox<S>::hasBeenUpdated(void); \
template Mailbox<S>::~Mailbox(); \
template Mailbox<S>::sotTimestampedObject& Mailbox<S>::get( Mailbox<S>::sotTimestampedObject& res,const int& dummy ); \
template Mailbox<S>::Mailbox(const std::string& name); \
}

} // namespace sot

#endif // #ifdef HAVE_LIBBOOST_THREAD
#endif // #ifdef __SOT_MAILBOX_T_CPP

