/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50173
Source Host           : localhost:3306
Source Database       : main

Target Server Type    : MYSQL
Target Server Version : 50173
File Encoding         : 65001

Date: 2016-03-31 10:46:38
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for auth_group
-- ----------------------------
DROP TABLE IF EXISTS `auth_group`;
CREATE TABLE `auth_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(80) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auth_group_permissions
-- ----------------------------
DROP TABLE IF EXISTS `auth_group_permissions`;
CREATE TABLE `auth_group_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `group_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `auth_group_permissions_group_id_0cd325b0_uniq` (`group_id`,`permission_id`),
  KEY `auth_group_permissi_permission_id_84c5c92e_fk_auth_permission_id` (`permission_id`),
  CONSTRAINT `auth_group_permissions_group_id_b120cbf9_fk_auth_group_id` FOREIGN KEY (`group_id`) REFERENCES `auth_group` (`id`),
  CONSTRAINT `auth_group_permissi_permission_id_84c5c92e_fk_auth_permission_id` FOREIGN KEY (`permission_id`) REFERENCES `auth_permission` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auth_permission
-- ----------------------------
DROP TABLE IF EXISTS `auth_permission`;
CREATE TABLE `auth_permission` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `content_type_id` int(11) NOT NULL,
  `codename` varchar(100) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `auth_permission_content_type_id_01ab375a_uniq` (`content_type_id`,`codename`),
  CONSTRAINT `auth_permissi_content_type_id_2f476e4b_fk_django_content_type_id` FOREIGN KEY (`content_type_id`) REFERENCES `django_content_type` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=67 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auth_user
-- ----------------------------
DROP TABLE IF EXISTS `auth_user`;
CREATE TABLE `auth_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `password` varchar(128) NOT NULL,
  `last_login` datetime DEFAULT NULL,
  `is_superuser` tinyint(1) NOT NULL,
  `username` varchar(30) NOT NULL,
  `first_name` varchar(30) NOT NULL,
  `last_name` varchar(30) NOT NULL,
  `email` varchar(254) NOT NULL,
  `is_staff` tinyint(1) NOT NULL,
  `is_active` tinyint(1) NOT NULL,
  `date_joined` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auth_user_groups
-- ----------------------------
DROP TABLE IF EXISTS `auth_user_groups`;
CREATE TABLE `auth_user_groups` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `auth_user_groups_user_id_94350c0c_uniq` (`user_id`,`group_id`),
  KEY `auth_user_groups_group_id_97559544_fk_auth_group_id` (`group_id`),
  CONSTRAINT `auth_user_groups_group_id_97559544_fk_auth_group_id` FOREIGN KEY (`group_id`) REFERENCES `auth_group` (`id`),
  CONSTRAINT `auth_user_groups_user_id_6a12ed8b_fk_auth_user_id` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for auth_user_user_permissions
-- ----------------------------
DROP TABLE IF EXISTS `auth_user_user_permissions`;
CREATE TABLE `auth_user_user_permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `auth_user_user_permissions_user_id_14a6b632_uniq` (`user_id`,`permission_id`),
  KEY `auth_user_user_perm_permission_id_1fbb5f2c_fk_auth_permission_id` (`permission_id`),
  CONSTRAINT `auth_user_user_permissions_user_id_a95ead1b_fk_auth_user_id` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`),
  CONSTRAINT `auth_user_user_perm_permission_id_1fbb5f2c_fk_auth_permission_id` FOREIGN KEY (`permission_id`) REFERENCES `auth_permission` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for django_admin_log
-- ----------------------------
DROP TABLE IF EXISTS `django_admin_log`;
CREATE TABLE `django_admin_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `action_time` datetime NOT NULL,
  `object_id` longtext,
  `object_repr` varchar(200) NOT NULL,
  `action_flag` smallint(5) unsigned NOT NULL,
  `change_message` longtext NOT NULL,
  `content_type_id` int(11) DEFAULT NULL,
  `user_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `django_admin__content_type_id_c4bce8eb_fk_django_content_type_id` (`content_type_id`),
  KEY `django_admin_log_user_id_c564eba6_fk_auth_user_id` (`user_id`),
  CONSTRAINT `django_admin_log_user_id_c564eba6_fk_auth_user_id` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`),
  CONSTRAINT `django_admin__content_type_id_c4bce8eb_fk_django_content_type_id` FOREIGN KEY (`content_type_id`) REFERENCES `django_content_type` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for django_content_type
-- ----------------------------
DROP TABLE IF EXISTS `django_content_type`;
CREATE TABLE `django_content_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `app_label` varchar(100) NOT NULL,
  `model` varchar(100) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `django_content_type_app_label_76bd3d3b_uniq` (`app_label`,`model`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for django_migrations
-- ----------------------------
DROP TABLE IF EXISTS `django_migrations`;
CREATE TABLE `django_migrations` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `app` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `applied` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for django_session
-- ----------------------------
DROP TABLE IF EXISTS `django_session`;
CREATE TABLE `django_session` (
  `session_key` varchar(40) NOT NULL,
  `session_data` longtext NOT NULL,
  `expire_date` datetime NOT NULL,
  PRIMARY KEY (`session_key`),
  KEY `django_session_de54fa62` (`expire_date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_keywordclasslist
-- ----------------------------
DROP TABLE IF EXISTS `strategy_keywordclasslist`;
CREATE TABLE `strategy_keywordclasslist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` varchar(200) NOT NULL,
  `level` int(11) NOT NULL,
  `direction` int(11) NOT NULL,
  `name` varchar(63) NOT NULL,
  `regex` varchar(127) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_keywordlist
-- ----------------------------
DROP TABLE IF EXISTS `strategy_keywordlist`;
CREATE TABLE `strategy_keywordlist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` varchar(200) NOT NULL,
  `level` int(11) NOT NULL,
  `direction` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_keywords
-- ----------------------------
DROP TABLE IF EXISTS `strategy_keywords`;
CREATE TABLE `strategy_keywords` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `keyword` varchar(127) NOT NULL,
  `list_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `strategy_keywords_list_id_6a2caf00_fk_strategy_keywordlist_id` (`list_id`),
  CONSTRAINT `strategy_keywords_list_id_6a2caf00_fk_strategy_keywordlist_id` FOREIGN KEY (`list_id`) REFERENCES `strategy_keywordlist` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_netgatedomain
-- ----------------------------
DROP TABLE IF EXISTS `strategy_netgatedomain`;
CREATE TABLE `strategy_netgatedomain` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `domain` varchar(200) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_spamlist
-- ----------------------------
DROP TABLE IF EXISTS `strategy_spamlist`;
CREATE TABLE `strategy_spamlist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` varchar(200) NOT NULL,
  `level` int(11) NOT NULL,
  `field` varchar(127) NOT NULL,
  `value` varchar(255) NOT NULL,
  `type` int(11) NOT NULL,
  `direction` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_strategy
-- ----------------------------
DROP TABLE IF EXISTS `strategy_strategy`;
CREATE TABLE `strategy_strategy` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` varchar(200) NOT NULL,
  `level` int(11) NOT NULL,
  `direction` int(11) NOT NULL,
  `spam` varchar(15) NOT NULL,
  `virus` varchar(15) NOT NULL,
  `keyword` varchar(15) NOT NULL,
  `keywordClass` varchar(15) NOT NULL,
  `url` varchar(15) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_urllist
-- ----------------------------
DROP TABLE IF EXISTS `strategy_urllist`;
CREATE TABLE `strategy_urllist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` varchar(200) NOT NULL,
  `level` int(11) NOT NULL,
  `direction` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_urls
-- ----------------------------
DROP TABLE IF EXISTS `strategy_urls`;
CREATE TABLE `strategy_urls` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `URL` varchar(255) NOT NULL,
  `list_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `strategy_urls_list_id_1ff70058_fk_strategy_urllist_id` (`list_id`),
  CONSTRAINT `strategy_urls_list_id_1ff70058_fk_strategy_urllist_id` FOREIGN KEY (`list_id`) REFERENCES `strategy_urllist` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for strategy_viruslist
-- ----------------------------
DROP TABLE IF EXISTS `strategy_viruslist`;
CREATE TABLE `strategy_viruslist` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` varchar(200) NOT NULL,
  `level` int(11) NOT NULL,
  `field` varchar(127) NOT NULL,
  `value` varchar(255) NOT NULL,
  `type` int(11) NOT NULL,
  `direction` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for syssuperuser_mailstatistic
-- ----------------------------
DROP TABLE IF EXISTS `syssuperuser_mailstatistic`;
CREATE TABLE `syssuperuser_mailstatistic` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` varchar(30) NOT NULL,
  `protocol` varchar(30) NOT NULL,
  `direction` varchar(4) NOT NULL,
  `num` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for syssuperuser_modulestatus
-- ----------------------------
DROP TABLE IF EXISTS `syssuperuser_modulestatus`;
CREATE TABLE `syssuperuser_modulestatus` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `status` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for syssuperuser_networksendrecv
-- ----------------------------
DROP TABLE IF EXISTS `syssuperuser_networksendrecv`;
CREATE TABLE `syssuperuser_networksendrecv` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `networkSend` bigint(20) NOT NULL,
  `networkRecv` bigint(20) NOT NULL,
  `timestamp` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for syssuperuser_operationreport
-- ----------------------------
DROP TABLE IF EXISTS `syssuperuser_operationreport`;
CREATE TABLE `syssuperuser_operationreport` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `time` datetime NOT NULL,
  `protocol` varchar(20) NOT NULL,
  `strategy` varchar(50) NOT NULL,
  `reason` varchar(50) NOT NULL,
  `detail` varchar(200) NOT NULL,
  `sourceip` varchar(50) NOT NULL,
  `destip` varchar(50) NOT NULL,
  `sender` varchar(100) NOT NULL,
  `receiver` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for syssuperuser_sysstatelog
-- ----------------------------
DROP TABLE IF EXISTS `syssuperuser_sysstatelog`;
CREATE TABLE `syssuperuser_sysstatelog` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `cpuPercent` double NOT NULL,
  `memPercent` double NOT NULL,
  `networkSendRate` double NOT NULL,
  `networkRecvRate` double NOT NULL,
  `timestamp` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for usermanage_useroperationlog
-- ----------------------------
DROP TABLE IF EXISTS `usermanage_useroperationlog`;
CREATE TABLE `usermanage_useroperationlog` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) NOT NULL,
  `rolename` varchar(60) NOT NULL,
  `domain` varchar(200) NOT NULL,
  `operation` varchar(400) NOT NULL,
  `optime` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for usermanage_userrole
-- ----------------------------
DROP TABLE IF EXISTS `usermanage_userrole`;
CREATE TABLE `usermanage_userrole` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(30) NOT NULL,
  `rolename` varchar(60) NOT NULL,
  `domain` varchar(200) NOT NULL,
  `realname` varchar(200) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
