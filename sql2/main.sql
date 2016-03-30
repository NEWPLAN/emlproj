/*
Navicat SQLite Data Transfer

Source Server         : db4
Source Server Version : 30808
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30808
File Encoding         : 65001

Date: 2016-03-30 14:29:49
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for auth_group
-- ----------------------------
DROP TABLE IF EXISTS "main"."auth_group";
CREATE TABLE "auth_group" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "name" varchar(80) NOT NULL UNIQUE);

-- ----------------------------
-- Table structure for auth_group_permissions
-- ----------------------------
DROP TABLE IF EXISTS "main"."auth_group_permissions";
CREATE TABLE "auth_group_permissions" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "group_id" integer NOT NULL REFERENCES "auth_group" ("id"), "permission_id" integer NOT NULL REFERENCES "auth_permission" ("id"));

-- ----------------------------
-- Table structure for auth_permission
-- ----------------------------
DROP TABLE IF EXISTS "main"."auth_permission";
CREATE TABLE "auth_permission" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "content_type_id" integer NOT NULL REFERENCES "django_content_type" ("id"), "codename" varchar(100) NOT NULL, "name" varchar(255) NOT NULL);

-- ----------------------------
-- Table structure for auth_user
-- ----------------------------
DROP TABLE IF EXISTS "main"."auth_user";
CREATE TABLE "auth_user" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "password" varchar(128) NOT NULL, "last_login" datetime NULL, "is_superuser" bool NOT NULL, "first_name" varchar(30) NOT NULL, "last_name" varchar(30) NOT NULL, "email" varchar(254) NOT NULL, "is_staff" bool NOT NULL, "is_active" bool NOT NULL, "date_joined" datetime NOT NULL, "username" varchar(30) NOT NULL UNIQUE);

-- ----------------------------
-- Table structure for auth_user_copy
-- ----------------------------
DROP TABLE IF EXISTS "main"."auth_user_copy";
CREATE TABLE "auth_user_copy" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "password" varchar(128) NOT NULL, "last_login" datetime NULL, "is_superuser" bool NOT NULL, "first_name" varchar(30) NOT NULL, "last_name" varchar(30) NOT NULL, "email" varchar(254) NOT NULL, "is_staff" bool NOT NULL, "is_active" bool NOT NULL, "date_joined" datetime NOT NULL, "username" varchar(30) NOT NULL UNIQUE);

-- ----------------------------
-- Table structure for auth_user_groups
-- ----------------------------
DROP TABLE IF EXISTS "main"."auth_user_groups";
CREATE TABLE "auth_user_groups" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "user_id" integer NOT NULL REFERENCES "auth_user" ("id"), "group_id" integer NOT NULL REFERENCES "auth_group" ("id"));

-- ----------------------------
-- Table structure for auth_user_user_permissions
-- ----------------------------
DROP TABLE IF EXISTS "main"."auth_user_user_permissions";
CREATE TABLE "auth_user_user_permissions" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "user_id" integer NOT NULL REFERENCES "auth_user" ("id"), "permission_id" integer NOT NULL REFERENCES "auth_permission" ("id"));

-- ----------------------------
-- Table structure for django_admin_log
-- ----------------------------
DROP TABLE IF EXISTS "main"."django_admin_log";
CREATE TABLE "django_admin_log" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "object_id" text NULL, "object_repr" varchar(200) NOT NULL, "action_flag" smallint unsigned NOT NULL, "change_message" text NOT NULL, "content_type_id" integer NULL REFERENCES "django_content_type" ("id"), "user_id" integer NOT NULL REFERENCES "auth_user" ("id"), "action_time" datetime NOT NULL);

-- ----------------------------
-- Table structure for django_content_type
-- ----------------------------
DROP TABLE IF EXISTS "main"."django_content_type";
CREATE TABLE "django_content_type" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "app_label" varchar(100) NOT NULL, "model" varchar(100) NOT NULL);

-- ----------------------------
-- Table structure for django_migrations
-- ----------------------------
DROP TABLE IF EXISTS "main"."django_migrations";
CREATE TABLE "django_migrations" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "app" varchar(255) NOT NULL, "name" varchar(255) NOT NULL, "applied" datetime NOT NULL);

-- ----------------------------
-- Table structure for django_session
-- ----------------------------
DROP TABLE IF EXISTS "main"."django_session";
CREATE TABLE "django_session" ("session_key" varchar(40) NOT NULL PRIMARY KEY, "session_data" text NOT NULL, "expire_date" datetime NOT NULL);

-- ----------------------------
-- Table structure for sqlite_sequence
-- ----------------------------
DROP TABLE IF EXISTS "main"."sqlite_sequence";
CREATE TABLE sqlite_sequence(name,seq);

-- ----------------------------
-- Table structure for strategy_keywordclasslist
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_keywordclasslist";
CREATE TABLE "strategy_keywordclasslist" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "owner" varchar(200) NOT NULL, "level" integer NOT NULL, "direction" integer NOT NULL, "name" varchar(63) NOT NULL, "regex" varchar(127) NOT NULL);

-- ----------------------------
-- Table structure for strategy_keywordlist
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_keywordlist";
CREATE TABLE "strategy_keywordlist" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "owner" varchar(200) NOT NULL, "level" integer NOT NULL, "direction" integer NOT NULL);

-- ----------------------------
-- Table structure for strategy_keywords
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_keywords";
CREATE TABLE "strategy_keywords" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "keyword" varchar(127) NOT NULL, "list_id" integer NOT NULL REFERENCES "strategy_keywordlist" ("id"));

-- ----------------------------
-- Table structure for strategy_netgatedomain
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_netgatedomain";
CREATE TABLE "strategy_netgatedomain" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "domain" varchar(200) NOT NULL);

-- ----------------------------
-- Table structure for strategy_spamlist
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_spamlist";
CREATE TABLE "strategy_spamlist" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "owner" varchar(200) NOT NULL, "level" integer NOT NULL, "field" varchar(127) NOT NULL, "value" varchar(255) NOT NULL, "type" integer NOT NULL, "direction" integer NOT NULL);

-- ----------------------------
-- Table structure for strategy_strategy
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_strategy";
CREATE TABLE "strategy_strategy" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "owner" varchar(200) NOT NULL, "level" integer NOT NULL, "direction" integer NOT NULL, "spam" varchar(15) NOT NULL, "keyword" varchar(15) NOT NULL, "keywordClass" varchar(15) NOT NULL, "url" varchar(15) NOT NULL, "virus" varchar(15) NOT NULL);

-- ----------------------------
-- Table structure for strategy_urllist
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_urllist";
CREATE TABLE "strategy_urllist" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "owner" varchar(200) NOT NULL, "level" integer NOT NULL, "direction" integer NOT NULL);

-- ----------------------------
-- Table structure for strategy_urls
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_urls";
CREATE TABLE "strategy_urls" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "URL" varchar(255) NOT NULL, "list_id" integer NOT NULL REFERENCES "strategy_urllist" ("id"));

-- ----------------------------
-- Table structure for strategy_viruslist
-- ----------------------------
DROP TABLE IF EXISTS "main"."strategy_viruslist";
CREATE TABLE "strategy_viruslist" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "owner" varchar(200) NOT NULL, "level" integer NOT NULL, "field" varchar(127) NOT NULL, "value" varchar(255) NOT NULL, "type" integer NOT NULL, "direction" integer NOT NULL);

-- ----------------------------
-- Table structure for syssuperuser_mailstatistic
-- ----------------------------
DROP TABLE IF EXISTS "main"."syssuperuser_mailstatistic";
CREATE TABLE "syssuperuser_mailstatistic" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "type" varchar(30) NOT NULL, "protocol" varchar(30) NOT NULL, "direction" varchar(4) NOT NULL, "num" integer NOT NULL);

-- ----------------------------
-- Table structure for syssuperuser_modulestatus
-- ----------------------------
DROP TABLE IF EXISTS "main"."syssuperuser_modulestatus";
CREATE TABLE "syssuperuser_modulestatus" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "name" varchar(100) NOT NULL, "status" bool NOT NULL);

-- ----------------------------
-- Table structure for syssuperuser_networksendrecv
-- ----------------------------
DROP TABLE IF EXISTS "main"."syssuperuser_networksendrecv";
CREATE TABLE "syssuperuser_networksendrecv" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "networkRecv" bigint NOT NULL, "networkSend" bigint NOT NULL, "timestamp" bigint NOT NULL);

-- ----------------------------
-- Table structure for syssuperuser_operationreport
-- ----------------------------
DROP TABLE IF EXISTS "main"."syssuperuser_operationreport";
CREATE TABLE "syssuperuser_operationreport" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "time" datetime NOT NULL, "protocol" varchar(20) NOT NULL, "strategy" varchar(50) NOT NULL, "reason" varchar(50) NOT NULL, "detail" varchar(200) NOT NULL, "sourceip" varchar(50) NOT NULL, "destip" varchar(50) NOT NULL, "sender" varchar(100) NOT NULL, "receiver" varchar(100) NOT NULL);

-- ----------------------------
-- Table structure for syssuperuser_sysstatelog
-- ----------------------------
DROP TABLE IF EXISTS "main"."syssuperuser_sysstatelog";
CREATE TABLE "syssuperuser_sysstatelog" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "cpuPercent" real NOT NULL, "memPercent" real NOT NULL, "networkRecvRate" real NOT NULL, "networkSendRate" real NOT NULL, "timestamp" bigint NOT NULL);

-- ----------------------------
-- Table structure for UserManage_useroperationlog
-- ----------------------------
DROP TABLE IF EXISTS "main"."UserManage_useroperationlog";
CREATE TABLE "UserManage_useroperationlog" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "username" varchar(30) NOT NULL, "rolename" varchar(60) NOT NULL, "domain" varchar(200) NOT NULL, "operation" varchar(400) NOT NULL, "optime" datetime NOT NULL);

-- ----------------------------
-- Table structure for UserManage_userrole
-- ----------------------------
DROP TABLE IF EXISTS "main"."UserManage_userrole";
CREATE TABLE "UserManage_userrole" ("id" integer NOT NULL PRIMARY KEY AUTOINCREMENT, "rolename" varchar(60) NOT NULL, "domain" varchar(200) NOT NULL, "realname" varchar(200) NOT NULL, "username" varchar(30) NOT NULL);

-- ----------------------------
-- Indexes structure for table auth_group_permissions
-- ----------------------------
CREATE INDEX "main"."auth_group_permissions_0e939a4f"
ON "auth_group_permissions" ("group_id" ASC);
CREATE INDEX "main"."auth_group_permissions_8373b171"
ON "auth_group_permissions" ("permission_id" ASC);
CREATE UNIQUE INDEX "main"."auth_group_permissions_group_id_0cd325b0_uniq"
ON "auth_group_permissions" ("group_id" ASC, "permission_id" ASC);

-- ----------------------------
-- Indexes structure for table auth_permission
-- ----------------------------
CREATE INDEX "main"."auth_permission_417f1b1c"
ON "auth_permission" ("content_type_id" ASC);
CREATE UNIQUE INDEX "main"."auth_permission_content_type_id_01ab375a_uniq"
ON "auth_permission" ("content_type_id" ASC, "codename" ASC);

-- ----------------------------
-- Indexes structure for table auth_user_groups
-- ----------------------------
CREATE INDEX "main"."auth_user_groups_0e939a4f"
ON "auth_user_groups" ("group_id" ASC);
CREATE INDEX "main"."auth_user_groups_e8701ad4"
ON "auth_user_groups" ("user_id" ASC);
CREATE UNIQUE INDEX "main"."auth_user_groups_user_id_94350c0c_uniq"
ON "auth_user_groups" ("user_id" ASC, "group_id" ASC);

-- ----------------------------
-- Indexes structure for table auth_user_user_permissions
-- ----------------------------
CREATE INDEX "main"."auth_user_user_permissions_8373b171"
ON "auth_user_user_permissions" ("permission_id" ASC);
CREATE INDEX "main"."auth_user_user_permissions_e8701ad4"
ON "auth_user_user_permissions" ("user_id" ASC);
CREATE UNIQUE INDEX "main"."auth_user_user_permissions_user_id_14a6b632_uniq"
ON "auth_user_user_permissions" ("user_id" ASC, "permission_id" ASC);

-- ----------------------------
-- Indexes structure for table django_admin_log
-- ----------------------------
CREATE INDEX "main"."django_admin_log_417f1b1c"
ON "django_admin_log" ("content_type_id" ASC);
CREATE INDEX "main"."django_admin_log_e8701ad4"
ON "django_admin_log" ("user_id" ASC);

-- ----------------------------
-- Indexes structure for table django_content_type
-- ----------------------------
CREATE UNIQUE INDEX "main"."django_content_type_app_label_76bd3d3b_uniq"
ON "django_content_type" ("app_label" ASC, "model" ASC);

-- ----------------------------
-- Indexes structure for table django_session
-- ----------------------------
CREATE INDEX "main"."django_session_de54fa62"
ON "django_session" ("expire_date" ASC);

-- ----------------------------
-- Indexes structure for table strategy_keywords
-- ----------------------------
CREATE INDEX "main"."strategy_keywords_4da3e820"
ON "strategy_keywords" ("list_id" ASC);

-- ----------------------------
-- Indexes structure for table strategy_urls
-- ----------------------------
CREATE INDEX "main"."strategy_urls_4da3e820"
ON "strategy_urls" ("list_id" ASC);
