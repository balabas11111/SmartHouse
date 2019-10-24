package com.balabas.smarthouse.server.entity.repository;

import java.util.List;

import javax.persistence.NoResultException;
import javax.persistence.NonUniqueResultException;

import org.hibernate.Criteria;
import org.hibernate.query.Query;

import com.balabas.smarthouse.server.entity.model.ItemAbstract;

import org.hibernate.query.NativeQuery;
import org.hibernate.Session;

public interface AbstractDAO<T extends ItemAbstract> {
	
	public Session getSession();
	
	public Query<?> getNamedQuery(String name);
	
	public void saveOrUpdate(T item);
	
	public void add(T item);
	
	public void update(T item);
	
	public void delete(Long id);
	
	public T getUniqueByHQL(String hql);

	public List<T> getListByHQL(String hql);
	
	public List<T> getList();
	
	public T findUniqueByCriterias(Criteria criteria)
			throws NonUniqueResultException, NoResultException;
	
	public T get(Long id);
	
	public Criteria createSimpleCriteria();
	
	public Query<?> createHQLQuery(String query);
	
	public Query<?> createHQLQuery(String query,@SuppressWarnings("rawtypes") Class clazz);
	
	public NativeQuery<?> createSQLQuery(String query);
	
	public T getUniqueByName(String name);
	
	public void saveOrUpdate(T item,Session session);
	
	public T initItem(T item);

	List<T> initItems(List<T> items);
	
	public List<T> getOrderedBy(String fieldName,boolean asc);
	
}
