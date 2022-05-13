package process

import (
	"commsys/common/message"
	_ "fmt"
)

// 客户端需要维护的map
var MyOnlineUsers map[string]*message.User = make(map[string]*message.User, 10)
