package model

import (
	"encoding/json"
	"fmt"
	"github.com/go-redis/redis"
)

// 完成对user结构体的操作
// 全局只有一个UserDao实例

var (
	MyUserDao *UserDao
)

type UserDao struct {
	redisDb *redis.Client
}

func NewUserDao() (ud *UserDao) {
	ud = &UserDao{}
	ud.Init()
	return
}

func (ud *UserDao) Init() error {

	ud.redisDb = redis.NewClient(&redis.Options{
		Addr:     "localhost:6379", // redis地址
		Password: "",               // redis密码，没有则留空
		DB:       0,                // 默认数据库，默认是0
	})

	//通过 *redis.Client.Ping() 来检查是否成功连接到了redis服务器
	_, err := ud.redisDb.Ping().Result()

	if err != nil {
		return err
	}
	return err
}

// 1. 根据用户Id 返回User实例+err
func (ud *UserDao) getUserById(id string) (user *User, err error) {
	// 通过给定的id 去 redis查询该用户
	userIns, err := ud.redisDb.HGet("users", id).Result()

	if err != nil {
		if err == redis.Nil {
			err = ErrorUserNotExists
		}
		return
	}

	user = &User{}
	err = json.Unmarshal([]byte(userIns), user)
	if err != nil {
		return
	}
	return
}

// 完成登录校验
// 1. 完成对用户的验证：id,pwd都正确返回user实例，其他返回错误
func (ud *UserDao) LoginCheck(id string, pwd string) (user *User, err error) {
	// 通过给定的id 去 redis查询该用户
	user, err = ud.getUserById(id)

	// 判断用户存在与否
	if err != nil {
		return
	}

	if user.UserPwd != pwd {
		err = ErrorWrongPwd
		return
	}
	return
}

// 注册逻辑

func (ud *UserDao) RegisterCheck(user *User) (err error) {
	_, err = ud.getUserById(user.UserId)

	// 用户已存在
	if err == nil {
		err = ErrorUserExists
		return
	}

	// 序列化
	data, err := json.Marshal(user)
	if err != nil {
		return
	}

	// 数据放入数据库
	err = ud.redisDb.HSet("users", user.UserId, string(data)).Err()
	if err != nil {
		fmt.Println("保存注册用户错误 err = ", err)
		return
	}
	return
}
