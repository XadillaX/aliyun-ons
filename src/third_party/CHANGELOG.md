## v1.0.8 - 2016/12/02

1. feat: add **oneway** sending method;
2. feat: add **ordered message** sending method;
3. feat: setting for sending timeout;
4. feat: setting for sending retry times;
5. fix: memory leak when `shutdown`;
6. fix: coredump of `shutdown`;
7. chore(boost): update to 1.6.2.

## v1.0.7 - 2016/11/15

1. feat: 1000 limiting for pulling messages;
2. feat: trace for messages;
3. fix: several bugs of tracing;
4. fix: memory leak;
5. fix: several bugs;
6. fix: bugs when tag include `||`;
7. refactor: optimization for TCP lock.
