package com.balabas.smarthouse.server.entity.repository;

import org.springframework.data.repository.CrudRepository;
import org.springframework.stereotype.Repository;

import com.balabas.smarthouse.server.view.chart.Metric;

@Repository
public interface IViewChartEntityFieldsRepository extends CrudRepository<Metric, Long> {

}
