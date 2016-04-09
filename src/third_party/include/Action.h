#ifndef __ACTION_H__
#define __ACTION_H__

//consuming result
enum Action{
//consume success, application could continue to consume next message
CommitMessage,
//consume fail, server will deliver this message later, application could continue to consume next message
ReconsumeLater
};
#endif

