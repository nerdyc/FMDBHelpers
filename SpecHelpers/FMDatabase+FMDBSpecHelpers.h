#import "FMDatabase.h"
#import "FMDatabase+FMDBHelpers.h"

@interface FMDatabase (FMDBSpecHelpers)

// ========== OPEN =====================================================================================================
#pragma mark - Open

+ (instancetype)openTemporaryDatabase;
+ (instancetype)openInMemoryDatabase;
+ (instancetype)openDatabaseWithPath:(NSString *)path;

- (void)openOrFail;

// ========== TABLES ===================================================================================================
#pragma mark - Tables

- (void)createTableWithName:(NSString *)tableName
                    columns:(NSArray *)columns;

- (void)createTableWithName:(NSString *)tableName
                    columns:(NSArray *)columns
                constraints:(NSArray *)constraints;

- (void)renameTable:(NSString *)tableName
                 to:(NSString *)newTableName;

- (void)addColumn:(NSString *)columnDefinition
          toTable:(NSString *)tableName;

- (void)dropTableWithName:(NSString *)tableName;
- (void)dropTableIfExistsWithName:(NSString *)tableName;

// ========== INDEXES ==================================================================================================
#pragma mark - Indexes

- (void)createIndexWithName:(NSString *)indexName
                  tableName:(NSString *)tableName
                    columns:(NSArray *)columns;

- (void)createUniqueIndexWithName:(NSString *)indexName
                        tableName:(NSString *)tableName
                          columns:(NSArray *)columns;

- (void)dropIndexWithName:(NSString *)indexName;

// ========== INSERT ===================================================================================================
#pragma mark - Insert

- (void)insertInto:(NSString *)tableName
           columns:(NSArray *)columns
            values:(NSArray *)values;

// ========== COUNT ====================================================================================================
#pragma mark - Count

- (NSInteger)countFrom:(NSString *)tableName;

// ========== SELECT ===================================================================================================
#pragma mark - Select

- (NSArray *)selectAllFrom:(NSString *)tableName
                   orderBy:(NSString *)orderBy;

// ========== UPDATE ===================================================================================================
#pragma mark - Update

- (NSInteger)update:(NSString *)tableName
          setValues:(NSDictionary *)setOfValues
              where:(NSString *)where
          arguments:(NSArray *)arguments;

- (NSInteger)update:(NSString *)tableName
                set:(NSDictionary *)setOfExpressions
              where:(NSString *)where
          arguments:(NSArray *)arguments;

// ========== DELETE ===================================================================================================
#pragma mark - Delete

- (NSInteger)deleteFrom:(NSString *)tableName
                  where:(NSString *)where
              arguments:(NSArray *)arguments;

@end
