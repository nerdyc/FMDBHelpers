#import "FMDatabase+FMDBSpecHelpers.h"
#import <FMDB/FMDatabaseAdditions.h>

#define UNEXPECTED_ERROR(__ERROR__, __MESSAGE__, ...) \
{ \
  @throw [NSException exceptionWithName:NSInternalInconsistencyException \
                                 reason:[NSString stringWithFormat:@"%@: %@", \
                                            [NSString stringWithFormat:(__MESSAGE__), ##__VA_ARGS__], \
                                            (__ERROR__)] \
                               userInfo:nil]; \
}

@implementation FMDatabase (FMDBSpecHelpers)

// ========== OPEN =====================================================================================================
#pragma mark - Open

+ (instancetype)openTemporaryDatabase
{
  return [self openDatabaseWithPath:@""];
}

+ (instancetype)openInMemoryDatabase
{
  return [self openDatabaseWithPath:nil];
}

+ (instancetype)openDatabaseWithPath:(NSString *)path
{
  FMDatabase * database = [self databaseWithPath:path];
  [database openOrFail];
  return database;
}

- (void)openOrFail
{
  NSError * error = nil;
  BOOL opened = [self open:&error];
  if (NO == opened)
  {
    UNEXPECTED_ERROR(error, @"Error opening temporary database");
  }
}

// ========== TABLES ===================================================================================================
#pragma mark - Tables

- (void)createTableWithName:(NSString *)tableName
                    columns:(NSArray *)columns
{
  [self createTableWithName:tableName
                    columns:columns
                constraints:nil];
}

- (void)createTableWithName:(NSString *)tableName
                    columns:(NSArray *)columns
                constraints:(NSArray *)constraints
{
  NSError * error = nil;
  BOOL created = [self createTableWithName:tableName
                                   columns:columns
                               constraints:constraints
                                     error:&error];
  if (!created)
  {
    UNEXPECTED_ERROR(error,
                     @"Error creating table {name:%@, columns:%@, constraints:%@}",
                     tableName,
                     columns,
                     constraints);
  }
}

- (void)addColumn:(NSString *)columnDefinition
          toTable:(NSString *)tableName
{
  NSError * error = nil;
  BOOL added = [self addColumn:columnDefinition
                       toTable:tableName
                         error:&error];
  if (!added)
  {
    UNEXPECTED_ERROR(error,
                     @"Error adding column (%@) to table '%@'",
                     columnDefinition,
                     tableName);
  }
}

- (void)renameTable:(NSString *)tableName
                 to:(NSString *)newTableName
{
  NSError * error = nil;
  BOOL renamed = [self renameTable:tableName
                                to:newTableName
                             error:&error];
  if (!renamed)
  {
    UNEXPECTED_ERROR(error,
                     @"Error renaming table '%@' to '%@'",
                     tableName,
                     newTableName);
  }
}

- (void)dropTableWithName:(NSString *)tableName
{
  NSError * error = nil;
  BOOL dropped = [self dropTableWithName:tableName
                                   error:&error];
  if (!dropped)
  {
    UNEXPECTED_ERROR(error, @"Error dropping table '%@'", tableName);
  }
}

- (void)dropTableIfExistsWithName:(NSString *)tableName
{
  NSError * error = nil;
  BOOL dropped = [self dropTableIfExistsWithName:tableName
                                           error:&error];
  if (!dropped)
  {
    UNEXPECTED_ERROR(error, @"Error dropping table '%@'", tableName);
  }
}

// ========== INDEXES ==================================================================================================
#pragma mark - Indexes

- (void)createIndexWithName:(NSString *)indexName
                  tableName:(NSString *)tableName
                    columns:(NSArray *)columns
{
  NSError * error = nil;
  
  BOOL created = [self createIndexWithName:indexName
                                 tableName:tableName
                                   columns:columns
                                     error:&error];
  if (!created)
  {
    UNEXPECTED_ERROR(error, @"Error creating index '%@' on table '%@' %@", indexName, tableName, columns);
  }
}

- (void)createUniqueIndexWithName:(NSString *)indexName
                        tableName:(NSString *)tableName
                          columns:(NSArray *)columns
{
  NSError * error = nil;
  
  BOOL created = [self createUniqueIndexWithName:indexName
                                       tableName:tableName
                                         columns:columns
                                           error:&error];
  if (!created)
  {
    UNEXPECTED_ERROR(error, @"Error creating unique index '%@' on table '%@' %@", indexName, tableName, columns);
  }
}

- (void)dropIndexWithName:(NSString *)indexName
{
  NSError * error = nil;
  BOOL dropped = [self dropIndexWithName:indexName
                                   error:&error];
  if (!dropped)
  {
    UNEXPECTED_ERROR(error, @"Error dropping index '%@'", indexName);
  }
}

- (NSString *)sqlForIndexWithName:(NSString *)indexName
{
  FMResultSet * schema = [self getSchema];
  while ([schema next])
  {
    NSString * type = [schema stringForColumn:@"type"];
    if ([type isEqualToString:@"index"] && [[schema stringForColumn:@"name"] isEqualToString:indexName])
    {
      NSString * sql = [schema stringForColumn:@"sql"];
      [schema close];
      return sql;
    }
  }
  
  return nil;
}

// ========== INSERT ===================================================================================================
#pragma mark - Insert

- (void)insertInto:(NSString *)tableName
           columns:(NSArray *)columns
            values:(NSArray *)values
{
  NSError * error = nil;
  BOOL succeeded = [self insertInto:tableName
                            columns:columns
                             values:values
                              error:&error];
  if (!succeeded)
  {
    UNEXPECTED_ERROR(error, @"Error inserting into '%@'", tableName);
  }
}

// ========== COUNT ====================================================================================================
#pragma mark - Count

- (NSInteger)countFrom:(NSString *)from
{
  NSError * error = nil;
  NSInteger count = [self countFrom:from
                              error:&error];
  if (count < 0) {
    UNEXPECTED_ERROR(error, @"Error counting '%@'", from);
  }
  
  return count;
}

// ========== SELECT ===================================================================================================
#pragma mark - Select

- (NSArray *)selectAllFrom:(NSString *)tableName
                   orderBy:(NSString *)orderBy
{
  NSError * error = nil;
  NSArray * allRecords = [self selectAllFrom:tableName
                                     orderBy:orderBy
                                       error:&error];
  if (allRecords == nil)
  {
    UNEXPECTED_ERROR(error, @"Error selecting all records from '%@'", tableName);
  }
  
  return allRecords;
}

// ========== UPDATE ===================================================================================================
#pragma mark - Update

- (NSInteger)update:(NSString *)tableName
             values:(NSDictionary *)values
              where:(NSString *)where
          arguments:(NSArray *)arguments
{
  NSError * error = nil;
  NSInteger numberUpdated = [self update:tableName
                                  values:values
                                   where:where
                               arguments:arguments
                                   error:&error];
  if (numberUpdated < 0)
  {
    UNEXPECTED_ERROR(error, @"Error updating '%@'", tableName);
  }
  
  return numberUpdated;
}

- (NSInteger)update:(NSString *)tableName
            columns:(NSArray *)columnNames
        expressions:(NSArray *)expressions
              where:(NSString *)where
          arguments:(NSArray *)arguments
{
  NSError * error = nil;
  NSInteger numberUpdated = [self update:tableName
                                 columns:columnNames
                             expressions:expressions
                                   where:where
                               arguments:arguments
                                   error:&error];
  if (numberUpdated < 0)
  {
    UNEXPECTED_ERROR(error, @"Error updating '%@'", tableName);
  }
  
  return numberUpdated;
}

// ========== DELETE ===================================================================================================
#pragma mark - Delete

- (NSInteger)deleteFrom:(NSString *)tableName
                  where:(NSString *)where
              arguments:(NSArray *)arguments
{
  NSError * error = nil;
  NSInteger numberDeleted = [self deleteFrom:tableName
                                       where:where
                                   arguments:arguments
                                       error:&error];
  if (numberDeleted < 0)
  {
    UNEXPECTED_ERROR(error, @"Error deleting '%@'", tableName);
  }
  
  return numberDeleted;
}

@end
