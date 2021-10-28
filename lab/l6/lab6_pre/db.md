# Difference between Row oriented and Column oriented data stores

## Data store

A **data store** is basically a place for storing collections of data, such as a database, a file system or a directory. In Database system they can be stored in two ways. These are as follows:

1. Row Oriented Data Stores
2. Column-Oriented Data Stores

## Difference

|                   Row oriented data stores                   |                 Column oriented data stores                  |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| Data is stored and retrieved one row at a time and hence could read unnecessary data if some of the data in a row are required. | In this type of data stores, data are stored and retrieve in columns and hence it can only able to read only the relevant data if required. |
| Records in Row Oriented Data stores are easy to read and write. | In this type of data stores, read and write operations are slower as compared to row-oriented. |
| These are not efficient in performing operations applicable to the entire datasets and hence aggregation in row-oriented is an **expensive** job or operations. | These are efficient in performing operations applicable to the entire dataset and hence enables **aggregation** over many rows and columns. |
| Typical compression mechanisms which provide less efficient result than what we achieve from column-oriented data stores. | These type of data stores basically permits high compression rates due to **little distinct or unique values** in columns. |

## Examples

##### **Row oriented data stores**

**Relational Database**, including which is a structured data storage and also a sophisticated query engine. They use Sql language, which stands for *Structured Query Language* to query data. It incurs a big penalty to improve performance as the data size increases. Such databases include some of the most popular and widely used databases in the world, including Mysql, Oracle Database and so on.

Mysql             | Oracle 
:-------------------------:|:-------------------------:
![](C:\Users\Lenovo\Desktop\lab6_pre\mysql.png)  | ![](C:\Users\Lenovo\Desktop\lab6_pre\oracle.png) 

##### **Column-Oriented data stores**

**Apache HBase Database**, which is a NoSql, fault tolerant, distributed database as a part of the Apache Hadoop ecosystem. It supports very fast querying with an extremely large data size.

## Lemondb

In our case, lemondb is a **row-oriented database** because:

- we use Sql-like language to query data from lemondb;
- we traverse target tables one row by one row;
- each row has a unique **KEY**.

