*****************************************************************************************************************
This document discription how to use ons client4cpp.

1. After download aliyun-ons-client-cpp.zip, untar it
	download address: http://onsteam.oss-cn-hangzhou.aliyuncs.com/aliyun-ons-client-cpp.zip
2. aliyun-ons-client-cpp directory:
    lib:        librocketmq.so, librocketmq.a
    include:    header files
    example:    producer/pushconsumer example
    readme:     discription how to use ons client4cpp

3. use g++ to link metaq lib: librocketmq.so or librocketmq.a

=============Attentions==================================================================
Following is the mandatory interfaces for producer and consumer:

Mandatory interfaces of producer: 
Producer *pProducer = ONSFactory::getInstance()->createProducer(...);
pProducer->start();//start must be called before excuting send and shutdown functions
pProducer->send(...);
pProducer->shutdown();


Mandatory interfaces of pushconsumer:
PushConsumer *pConsumer = ONSFactory::getInstance()->createPushConsumer(...);
pConsumer->subscribe("test_topic", "tag", &msgListener);
pConsumer->start();//start must be called before excuting shutdown function
pConsumer->shutdown();


*******************************************************************************************************************
