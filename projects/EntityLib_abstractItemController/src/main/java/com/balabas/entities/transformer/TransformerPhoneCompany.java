package com.balabas.entities.transformer;

import org.springframework.stereotype.Service;

import com.balabas.abstr.transformer.AbstractTransformer;
import com.balabas.abstr.transformer.TransformerJson;
import com.balabas.entities.model.PhoneCompany;

@Service
public class TransformerPhoneCompany extends TransformerJson<PhoneCompany> implements AbstractTransformer<PhoneCompany> {

}
