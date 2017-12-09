# Envoy Auth Request Module

本项目实现了类似nginx的ngx_auth_request_module在envoy上。实现HTTP代理前的子请求验证和附加头信息。

> 由于envoy对websocket的支持问题，目前本模块不支持websocket。

## 编译

使用下面命令编译静态可执行文件，输出在：bazel-bin/envoy

1. `git submodule update --init`
2. `bazel build envoy`

## 配置

* 项目内`envoy.json`为参考配置文件。
* 其使用`localhost:8000`模拟上流服务器，`localhost:9000`模拟授权服务器，监听`localhost:8080`端口。
* 其示例服务器实现在toos目录下。

本模块配置如下

```
              {
                "type": "decoder",
                "name": "auth_request",
                "config": {
                  "upstream": "auth-upstream"
                }
              }
```

1. `type`，`name`为固定值，表示使用本模块。
2. `upstream`值为授权服务的clusters名称。

## Docker镜像

* 目前需要手动编译docker镜像并使用docker目录下Dockerfile打包。
* 有打包好的镜像在[https://hub.docker.com/r/xausky/envoy-auth-request/](https://hub.docker.com/r/xausky/envoy-auth-request/)。
* docker镜像使用需要先添加或者挂载配置文件进去，然后使用`envoy -c <配置文件>`来运行。
