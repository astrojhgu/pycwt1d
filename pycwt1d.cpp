#include <iostream>
#include <vector>
#include <boost/ref.hpp>
#include <boost/utility.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/def.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/operators.hpp>
#include <boost/python/numeric.hpp>
#include <boost/python/list.hpp>
#include <exception>
#define private public

using namespace boost;
using namespace boost::python;
using namespace boost::python::numeric;

#include <cwt1d_wavelets.hpp>
using namespace std;
using namespace cwt1d;

typedef dog<double> pydog;
typedef wavelet_func<double> wf;

namespace
{
  class initializer{
  public:
    initializer()
    {
      boost::python::numeric::array::set_module_and_type("numpy","ndarray");
      cout<<boost::python::numeric::array::get_module_name()<<endl;
    }
  }_init;
}

boost::python::numeric::array pycwt(const boost::python::numeric::array& x,const boost::python::numeric::array& s,const wavelet_func<double>& wf)
{
  boost::python::object shape(x.attr("shape"));
  int ndim=extract<int>(shape.attr("__len__")());
  if(ndim!=1)
    {
      throw std::exception();
    }
  int ndata=extract<int>(shape[0]);
  
  blitz::Array<double,1> x1(ndata);
  for(int i=0;i<ndata;++i)
    {
      x1(i)=extract<double>(x[i]);
    }
  shape=s.attr("shape");
  ndim=extract<int>(shape.attr("__len__")());
  if(ndim!=1)
    {
      throw std::exception();
    }
  int nscales=extract<int>(shape[0]);
  blitz::Array<double,1> s1(nscales);

  for(int i=0;i<nscales;++i)
    {
      s1(i)=extract<double>(s[i]);
    }

  blitz::Array<complex<double>,2> y(cwt(x1,s1,wf));
  boost::python::list l;
  for(int i=0;i<nscales;++i)
    {
      boost::python::list l1;
      for(int j=0;j<ndata;++j)
	{
	  l1.append(y(i,j));
	}
      l.append(l1);
    }
  
  boost::python::numeric::array result(l);

  //result.setshape(nscales,ndata);
  cout<<extract<int>(shape[0])<<endl;
  //cout<<o[0]<<endl;
  return result;
}

boost::python::numeric::array pyicwt(const boost::python::numeric::array& x,const boost::python::numeric::array& s,const wavelet_func<double>& wf)
{
  boost::python::object shape(x.attr("shape"));
  int ndim=extract<int>(shape.attr("__len__")());
  if(ndim!=2)
    {
      throw std::exception();
    }
  int ndata=extract<int>(shape[1]);
  int nscales=extract<int>(shape[0]);
  
  blitz::Array<complex<double>,2> x1(nscales,ndata);
  for(int i=0;i<nscales;++i)
    {
      for(int j=0;j<ndata;++j)
	{
	  x1(i,j)=extract<complex<double> >(x[boost::python::make_tuple(i,j)]);
	}
    }
  shape=s.attr("shape");
  ndim=extract<int>(shape.attr("__len__")());
  if(ndim!=1)
    {
      throw std::exception();
    }
  if(nscales!=extract<int>(shape[0]))
    {
      throw std::exception();
    }
  blitz::Array<double,1> s1(nscales);

  for(int i=0;i<nscales;++i)
    {
      s1(i)=extract<double>(s[i]);
    }
  blitz::Array<double,1> result1(icwt(x1,s1,wf));
  boost::python::list l;
  for(int i=0;i<ndata;++i)
    {
      l.append(result1(i));
    }
  boost::python::numeric::array result(l);
  return result;
}

double pycalc_norm(int dl,const boost::python::numeric::array& s,const wavelet_func<double>& wf)
{
  boost::python::object shape(s.attr("shape"));
  int ndim=extract<int>(shape.attr("__len__")());
  if(ndim!=1)
    {
      throw std::exception();
    }
  int nscales=extract<int>(shape[0]);
  blitz::Array<double,1> s1(nscales);
  for(int i=0;i<nscales;++i)
    {
      s1(i)=extract<double>(s[i]);
    }
  return calc_norm(dl,s1,wf);
}

BOOST_PYTHON_MODULE(cwt1d)
{
  class_<wf,boost::noncopyable>("wf",no_init);
  
  class_<pydog,bases<wf> >("dog")
    .def(init<>())
    .def(init<int>());

  

  def("cwt",pycwt);
  def("icwt",pyicwt);
  def("calc_norm",pycalc_norm);
}
