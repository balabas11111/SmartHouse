package com.balabas.smarthouse.server;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

import io.swagger.v3.oas.models.Components;
import io.swagger.v3.oas.models.OpenAPI;
import io.swagger.v3.oas.models.info.Info;

@Configuration
public class SwaggerApiConfig {

	// https://www.dariawan.com/tutorials/spring/documenting-spring-boot-rest-api-springdoc-openapi-3/
	// http://localhost:8080/v3/api-docs
	// http://localhost:8080/swagger-ui.html

	@Value("${smarthouse.server.swg.api.name:SmartHouse server api}")
	private String apiName;

	@Value("${smarthouse.server.swg.api.description:SmartHouse server api description}")
	private String apiDescription;

	@Value("${smarthouse.server.swg.api.version:1.0}")
	private String apiVersion;

	@Bean
	public OpenAPI customOpenAPI() {
		return new OpenAPI().components(new Components()).info(new Info().title(apiName)
				.description(apiDescription));
	}
}
