package com.balabas.smarthouse.server.entity.repository;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.PersistenceUnit;

import org.jinq.jpa.JPAJinqStream;
import org.jinq.jpa.JinqJPAStreamProvider;
import org.springframework.stereotype.Component;

@Component
public class JinqSource {
  private JinqJPAStreamProvider streams;
  
  @PersistenceUnit
  public void setEntityManagerFactory(
      EntityManagerFactory emf) throws Exception {
    streams = new JinqJPAStreamProvider(emf);
    // Do any additional Jinq initialization needed here.
  }

  // Wrapper that passes through Jinq requests to Jinq
  public <U> JPAJinqStream<U> streamAll(
      EntityManager em, Class<U>entity) {
    return streams.streamAll(em, entity);
  }
  
}
