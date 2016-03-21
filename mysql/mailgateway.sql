/*
 Navicat Premium Data Transfer

 Source Server         : mac
 Source Server Type    : MySQL
 Source Server Version : 50629
 Source Host           : localhost
 Source Database       : mailgateway

 Target Server Type    : MySQL
 Target Server Version : 50629
 File Encoding         : utf-8

 Date: 03/02/2016 16:28:37 PM
*/
/*
	latin1
*/
SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
--  Table structure for `DLP_list_keywords`
-- ----------------------------
DROP TABLE IF EXISTS `DLP_list_keywords`;
CREATE TABLE `DLP_list_keywords` (
  `strategy_id` int(10) DEFAULT NULL,
  `strategy_type` int(10) DEFAULT NULL,
  `strategy_level` int(5) DEFAULT NULL,
  `strategy_info` varchar(255) DEFAULT NULL,
  `strategy_terminal` int(5) DEFAULT NULL,
  `strategy_target` varchar(255) DEFAULT NULL,
  `strategy_content` text DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `DLP_list_keywordsclass`
-- ----------------------------
DROP TABLE IF EXISTS `DLP_list_keywordsclass`;
CREATE TABLE `DLP_list_keywordsclass` (
  `strategy_id` int(10) DEFAULT NULL,
  `strategy_type` int(10) DEFAULT NULL,
  `strategy_level` int(5) DEFAULT NULL,
  `strategy_info` varchar(255) NOT NULL,
  `strategy_terminal` int(5) DEFAULT NULL,
  `strategy_target` varchar(255) DEFAULT NULL,
  `strategy_content` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Protocol_switch`
-- ----------------------------
DROP TABLE IF EXISTS `Protocol_switch`;
CREATE TABLE `Protocol_switch` (
  `protocol` varchar(255) DEFAULT NULL,
  `direction` varchar(255) DEFAULT NULL,
  `switch` int(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Statistic_DLP_URL`
-- ----------------------------
DROP TABLE IF EXISTS `Statistic_DLP_URL`;
CREATE TABLE `Statistic_DLP_URL` (
  `DLP_URL_id` int(11) DEFAULT NULL,
  `DLP_URL_eml_id` int(11) DEFAULT NULL,
  `DLP_URL_hitlist` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Statistic_DLP_keywords`
-- ----------------------------
DROP TABLE IF EXISTS `Statistic_DLP_keywords`;
CREATE TABLE `Statistic_DLP_keywords` (
  `DLP_keyword_id` int(11) DEFAULT NULL,
  `DLP_keyword_eml_id` int(11) DEFAULT NULL,
  `DLP_keyword_hitlist` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Statistic_Emlinfo_Basic`
-- ----------------------------
DROP TABLE IF EXISTS `Statistic_Emlinfo_Basic`;
CREATE TABLE `Statistic_Emlinfo_Basic` (
  `eml_id` int(11) DEFAULT NULL,
  `eml_name` varchar(255) DEFAULT NULL,
  `eml_from` varchar(255) DEFAULT NULL,
  `eml_to` varchar(255) DEFAULT NULL,
  `eml_from_to` varchar(255) DEFAULT NULL,
  `eml_attachment_name` varchar(255) DEFAULT NULL,
  `eml_attachment_type` varchar(255) NOT NULL,
  `eml_attachment_virus` int(11) DEFAULT NULL,
  `eml_isspam` int(11) DEFAULT NULL,
  `eml_DLPkeywordhit` int(11) DEFAULT NULL,
  `eml_DLPkeyclasshit` int(11) DEFAULT NULL,
  `eml_URLhit` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Statistic_Protocol`
-- ----------------------------
DROP TABLE IF EXISTS `Statistic_Protocol`;
CREATE TABLE `Statistic_Protocol` (
  `protocol` varchar(255) DEFAULT NULL,
  `source_ip` varchar(255) DEFAULT NULL,
  `dest_ip` varchar(255) DEFAULT NULL,
  `open_time` datetime DEFAULT NULL,
  `close_time` datetime DEFAULT NULL,
  `eml_file_path` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Statistic_Spam_Basic`
-- ----------------------------
DROP TABLE IF EXISTS `Statistic_Spam_Basic`;
CREATE TABLE `Statistic_Spam_Basic` (
  `spam_id` int(10) DEFAULT NULL,
  `spam_eml_path` varchar(255) DEFAULT NULL,
  `spam_eml_score` varchar(255) DEFAULT NULL,
  `spam_eml_info` varchar(255) DEFAULT NULL,
  `spam_eml_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Statistic_Virus_Basic`
-- ----------------------------
DROP TABLE IF EXISTS `Statistic_Virus_Basic`;
CREATE TABLE `Statistic_Virus_Basic` (
  `virus_id` int(11) DEFAULT NULL,
  `virus_eml_id` int(11) DEFAULT NULL,
  `virus_filename` varchar(255) DEFAULT NULL,
  `virus_info` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Statistic_keywordclass_Basic`
-- ----------------------------
DROP TABLE IF EXISTS `Statistic_keywordclass_Basic`;
CREATE TABLE `Statistic_keywordclass_Basic` (
  `DLP_keywordclass_id` int(11) DEFAULT NULL,
  `DLP_keywordclass_eml_id` varchar(255) DEFAULT NULL,
  `DLP_keywordclass_hitlist` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `Syslog`
-- ----------------------------
DROP TABLE IF EXISTS `Syslog`;
CREATE TABLE `Syslog` (
  `modular` varchar(255) DEFAULT NULL,
  `time` datetime DEFAULT NULL,
  `level` varchar(255) DEFAULT NULL,
  `content` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `URL_list`
-- ----------------------------
DROP TABLE IF EXISTS `URL_list`;
CREATE TABLE `URL_list` (
  `strategy_id` int(10) DEFAULT NULL,
  `strategy_type` varchar(255) DEFAULT NULL,
  `strategy_content` varchar(255) DEFAULT NULL,
  `URL_level` varchar(255) DEFAULT NULL,
  `level_info` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `behavior_log`
-- ----------------------------
DROP TABLE IF EXISTS `behavior_log`;
CREATE TABLE `behavior_log` (
  `time` datetime DEFAULT NULL,
  `level` varchar(255) DEFAULT NULL,
  `user_id` int(11) DEFAULT NULL,
  `object` varchar(255) DEFAULT NULL,
  `behavior` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `keyword_list`
-- ----------------------------
DROP TABLE IF EXISTS `keyword_list`;
CREATE TABLE `keyword_list` (
  `keyword` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `keywordclass_list`
-- ----------------------------
DROP TABLE IF EXISTS `keywordclass_list`;
CREATE TABLE `keywordclass_list` (
  `class_id` int(11) DEFAULT NULL,
  `class_name` varchar(255) DEFAULT NULL,
  `class_regex` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `spam_list`
-- ----------------------------
DROP TABLE IF EXISTS `spam_list`;
CREATE TABLE `spam_list` (
  `strategy_id` int(10) DEFAULT NULL,
  `strategy_type` int(10) DEFAULT NULL,
  `strategy_level` int(5) DEFAULT NULL,
  `strategy_info` varchar(255) DEFAULT NULL,
  `strategy_terminal` int(5) DEFAULT NULL,
  `strategy_target` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `user_info`
-- ----------------------------
DROP TABLE IF EXISTS `user_info`;
CREATE TABLE `user_info` (
  `user_id` varchar(255) DEFAULT NULL,
  `user_type` int(10) DEFAULT NULL,
  `user_name` varchar(255) DEFAULT NULL,
  `user_department` varchar(255) DEFAULT NULL,
  `user_tel` varchar(255) DEFAULT NULL,
  `user_passwd` varchar(255) DEFAULT NULL,
  `user_mailbox` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `virus_list`
-- ----------------------------
DROP TABLE IF EXISTS `virus_list`;
CREATE TABLE `virus_list` (
  `strategy_id` int(10) DEFAULT NULL,
  `strategy_type` int(10) DEFAULT NULL,
  `strategy_level` int(5) DEFAULT NULL,
  `strategy_info` varchar(255) DEFAULT NULL,
  `strategy_terminal` int(5) DEFAULT NULL,
  `strategy_target` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

SET FOREIGN_KEY_CHECKS = 1;
