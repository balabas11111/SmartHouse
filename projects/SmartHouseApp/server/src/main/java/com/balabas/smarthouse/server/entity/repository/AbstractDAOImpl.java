package com.balabas.smarthouse.server.entity.repository;

import java.lang.reflect.Method;
import java.lang.reflect.ParameterizedType;
import java.util.Iterator;
import java.util.List;

import javax.persistence.NoResultException;
import javax.persistence.NonUniqueResultException;
import javax.persistence.criteria.CriteriaQuery;

import lombok.extern.log4j.Log4j;
import lombok.extern.log4j.Log4j2;

import org.hibernate.Criteria;
import org.hibernate.Hibernate;
import org.hibernate.query.Query;
import org.hibernate.query.NativeQuery;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.collection.internal.PersistentSet;
import org.hibernate.criterion.Criterion;
import org.hibernate.criterion.Order;
import org.hibernate.criterion.Restrictions;
import org.springframework.beans.factory.annotation.Autowired;

import com.balabas.smarthouse.server.entity.model.ItemAbstract;

@Log4j2
public abstract class AbstractDAOImpl<T extends ItemAbstract> implements AbstractDAO<T> {

	@Autowired 
	private SessionFactory sessionFactory;
	
	protected Class<T> clazz;
	
	public void init(){
	}
	
	@SuppressWarnings("unchecked")
	public AbstractDAOImpl(){
		clazz = (Class<T>) ((ParameterizedType) getClass()
				.getGenericSuperclass()).getActualTypeArguments()[0];
	}
	
	public Session getSession(){
		Session result=sessionFactory.getCurrentSession();
		return result;
	}
	
	public Query getNamedQuery(String name){
		Query result=getSession().getNamedQuery(name);
		return result;
	}
	
	public void saveOrUpdate(T item){
		saveOrUpdate(item,getSession());
	}
	
	public void add(T item){
		add(item,getSession());
	}
	
	public void update(T item){
		update(item,getSession());
	}
	
	public void delete(Long id){
		
		T item =get(id);
		
		if(null!=item){
			delete(item,getSession());
		}
	}
	
	public List<T> getList(){
		List<T> result=getList(getSession());
		
		initItems(result);
		return result;
	}
	
	public Query createHQLQuery(String query,@SuppressWarnings("rawtypes") Class clazz){
		return createHQLQuery(query,getSession());
	}
	
	public Query createHQLQuery(String query){
		return createHQLQuery(query,getSession());
	}
		
	public NativeQuery createSQLQuery(String query){
		return createSQLQuery(query,getSession());
	}
	
	@SuppressWarnings("unchecked")
	public List<T> getListByHQL(String hql){
		return getSession().createQuery(hql).list();
	}
	
	@SuppressWarnings("unchecked")
	public T getUniqueByHQL(String hql){
		return (T) getSession().createQuery(hql).uniqueResult();
	}
	
	public T get(Long id){
		Criterion criterion = Restrictions.eq(ItemAbstract.PROP_ID, id);
		T result = findUniqueByCriterias(createSimpleCriteria().add(criterion));
		
		return result;
	}
	
	public T getUniqueByName(String name){
		
		Criterion criterion = Restrictions.eq(ItemAbstract.PROP_NAME, name);
		T result = findUniqueByCriterias(createSimpleCriteria().add(criterion));
			
		return result;
	}
	
	@SuppressWarnings("unchecked")
	public T findUniqueByCriterias(Criteria criteria)
			throws NonUniqueResultException, NoResultException {
		return (T) criteria.uniqueResult();
	}
	
	public Criteria createSimpleCriteria() {
		return getSession().createCriteria(getEntityClass());
	}
	
	public void saveOrUpdate(T item,Session session){
		session.saveOrUpdate(item);
	}
	
	public void add(T item,Session session){
		session.save(item);
	}
	
	public void update(T item,Session session){
		session.update(item);
	}
	
	public void delete(T item,Session session){
		session.delete(item);
	}
	
	private Class<T> getEntityClass() {
		return clazz;
	}
	
	public List<T> getList(Session session){
		CriteriaQuery<T> criteriaQuery = session.getCriteriaBuilder().createQuery(clazz);
        criteriaQuery.from(clazz);
        
		List<T> list = session.createQuery(criteriaQuery).getResultList();
		session.close();
		
		return list;
	}
	
	public Query createHQLQuery(String query,Session session){
		return session.createQuery(query);
	}
	
	public NativeQuery createSQLQuery(String query,Session session){
		return session.createSQLQuery(query);
	}
	
	public static boolean isGetter(Method method){
		  if(!method.getName().startsWith("get"))      return false;
		  if(method.getParameterTypes().length != 0)   return false;  
		  if(void.class.equals(method.getReturnType())) return false;
		  return true;
	}
	
	@SuppressWarnings("unchecked")
	private void checkMethod(Object value){
		if(value!=null  && value.getClass()!=null && value.getClass().getSuperclass()!=null)
		if(value.getClass().getSuperclass().equals(ItemAbstract.class)){
			initItem((T)value);
		}
	}
	
	public List<T> initItems(List<T> items){
		for(T item:items){
			initItem(item);
		}
		
		return items;
	}
	
	@SuppressWarnings("rawtypes")
	public T initItem(T item){
		
		if(item==null){
			return item;
		}
		
				try{
					Method[] methods = item.getClass().getMethods();

				for(Method method : methods)
					if(isGetter(method)){
			
						Hibernate.initialize(method.invoke(item,(Object[]) null));
						
						Object result=method.invoke(item);

						Class resultClass=method.getReturnType();
						
						if(resultClass.equals(PersistentSet.class)){
							Iterator iter=((PersistentSet) result).iterator();
							
							while(iter.hasNext()){
								Object res2=iter.next();
								
								checkMethod(res2);
							}
							
						}else{
							checkMethod(result);
						}
					}
				
				return (T) item;
				
				}catch(Exception e){
					log.error("Error initialize object "+e.getMessage());
				}
				
				return null;
	 
	}
	
	@SuppressWarnings("unchecked")
	public List<T> getOrderedBy(String fieldName,boolean asc){
		Criteria cr = getSession().createCriteria(clazz);
		
		if (asc){
			cr.addOrder(Order.asc(fieldName));
		}else{
			cr.addOrder(Order.desc(fieldName));
		}
		
		List<T> list=cr.list();
		
		return list;
	}
	
}

