#import "FMDatabase.h"

@interface FMDatabase (FMDBHelpers)

// ========== INITIALIZATION ===========================================================================================
#pragma mark - Initialization

+ (instancetype)temporaryDatabase;
+ (instancetype)inMemoryDatabase;

// ========== OPEN =====================================================================================================
#pragma mark - Open

- (BOOL)open:(NSError **)error_p;

- (BOOL)openWithFlags:(int)flags
                error:(NSError **)error_p;

// ========== ESCAPE ===================================================================================================
#pragma mark - Escape

+ (NSString *)escapeString:(NSString *)value;
+ (NSString *)escapeIdentifier:(NSString *)value;
+ (NSString *)escapeValue:(id)value;

// ========== UPDATES ==================================================================================================
#pragma mark - Updates

- (BOOL)executeUpdate:(NSString *)sql
                error:(NSError **)error_p;

- (BOOL)executeUpdate:(NSString *)sql
 withArgumentsInArray:(NSArray *)arguments
                error:(NSError **)error_p;

- (BOOL)  executeUpdate:(NSString *)sql
withParameterDictionary:(NSDictionary *)arguments
                  error:(NSError **)error_p;

// ========== SCHEMA ===================================================================================================
#pragma mark - Schema

- (NSDictionary *)databaseSchema;

// ========== TABLES ===================================================================================================
#pragma mark - Tables

- (NSDictionary *)tableSchema:(NSString *)tableName;
- (NSSet *)tableNames;

- (BOOL)createTableWithName:(NSString *)tableName
                    columns:(NSArray *)columns
                constraints:(NSArray *)constraints
                      error:(NSError **)error_p;

- (BOOL)renameTable:(NSString *)tableName
                 to:(NSString *)newTableName
              error:(NSError **)error_p;

- (BOOL)addColumn:(NSString *)columnDefinition
          toTable:(NSString *)tableName
            error:(NSError **)error_p;

- (BOOL)dropTableWithName:(NSString *)tableName
                    error:(NSError **)error_p;

- (BOOL)dropTableIfExistsWithName:(NSString *)tableName
                            error:(NSError **)error_p;

// ========== INDEXES ==================================================================================================
#pragma mark - Indexes

- (NSArray *)indexNamesOnTable:(NSString *)tableName;

- (BOOL)createIndexWithName:(NSString *)indexName
                  tableName:(NSString *)tableName
                    columns:(NSArray *)columns
                      error:(NSError **)error_p;

- (BOOL)createUniqueIndexWithName:(NSString *)indexName
                        tableName:(NSString *)tableName
                          columns:(NSArray *)columns
                            error:(NSError **)error_p;

- (BOOL)dropIndexWithName:(NSString *)indexName
                    error:(NSError **)error_p;

// ========== INSERT ===================================================================================================
#pragma mark - Insert

- (BOOL)insertInto:(NSString *)tableName
           columns:(NSArray *)columns
            values:(NSArray *)values
             error:(NSError **)error_p;

// ========== COUNT ====================================================================================================
#pragma mark - Count

- (NSInteger)countFrom:(NSString *)from
                 error:(NSError **)error_p;

- (NSInteger)count:(NSArray *)columnNames
              from:(NSString *)from
             where:(NSString *)where
         arguments:(NSArray *)arguments
             error:(NSError **)error_p;

- (NSInteger)count:(NSArray *)columnNames
              from:(NSString *)from
             where:(NSString *)where
        parameters:(NSDictionary *)parameters
             error:(NSError **)error_p;

// ========== SELECT ===================================================================================================
#pragma mark - Select

- (NSArray *)selectAllFrom:(NSString *)from
                   orderBy:(NSString *)orderBy
                     error:(NSError **)error_p;

- (NSArray *)selectAllFrom:(NSString *)from
                     where:(NSString *)where
                 arguments:(NSArray *)arguments
                   orderBy:(NSString *)orderBy
                     error:(NSError **)error_p;

// ---------- RESULTS --------------------------------------------------------------------------------------------------
#pragma mark Results

- (FMResultSet *)selectResultsFrom:(NSString *)from
                           orderBy:(NSString *)orderBy
                             error:(NSError **)error_p;

- (FMResultSet *)selectResultsFrom:(NSString *)from
                             where:(NSString *)where
                         arguments:(NSArray *)arguments
                           orderBy:(NSString *)orderBy
                             error:(NSError **)error_p;

- (FMResultSet *)selectResults:(NSArray *)columns
                          from:(NSString *)from
                         where:(NSString *)where
                       groupBy:(NSString *)groupBy
                        having:(NSString *)having
                     arguments:(NSArray *)arguments
                       orderBy:(NSString *)orderBy
                         limit:(NSNumber *)limit
                        offset:(NSNumber *)offset
                         error:(NSError **)error_p;

// ========== UPDATE ===================================================================================================
#pragma mark - Update

- (NSInteger)update:(NSString *)tableName
             values:(NSDictionary *)values
              where:(NSString *)where
          arguments:(NSArray *)arguments
              error:(NSError **)error_p;

- (NSInteger)update:(NSString *)tableName
            columns:(NSArray *)columnNames
        expressions:(NSArray *)expressions
              where:(NSString *)where
          arguments:(NSArray *)arguments
              error:(NSError **)error_p;

// ========== DELETE ===================================================================================================
#pragma mark - Delete

- (NSInteger)deleteFrom:(NSString *)tableName
                  where:(NSString *)where
              arguments:(NSArray *)arguments
                  error:(NSError **)error_p;

@end
