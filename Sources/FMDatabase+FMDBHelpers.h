#import "FMDatabase.h"

@interface FMDatabase (FMDBHelpers)

// ========== INITIALIZATION ===========================================================================================
#pragma mark - Initialization

/// @name Creating Databases

/**
 *  Create a new temporary database.
 */
+ (instancetype)temporaryDatabase;

/**
 *  Create a new in-memory database.
 */
+ (instancetype)inMemoryDatabase;

// ========== OPEN =====================================================================================================
#pragma mark - Open

/// @name Opening Databases

/**
 *  Open the database, or provide an error.
 *
 *  @param  error_p         A pointer to any error that occurs.
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)open:(NSError **)error_p;

/**
 *  Open the database with specific flags, or provide an error.
 *
 *  @param  error_p         A pointer to any error that occurs.
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)openWithFlags:(int)flags
                error:(NSError **)error_p;

// ========== ESCAPE ===================================================================================================
#pragma mark - Escape

/// @name Escaping Values

/**
 *  Returns an escaped version of the given string that can be added to a SQL statement.
 */
+ (NSString *)escapeString:(NSString *)value;

/**
 *  Escapes the given identifier (e.g. table name or column name). sqlite identifiers use single quotes.
 */
+ (NSString *)escapeIdentifier:(NSString *)value;

/**
 *  Escapes the given SQL value (e.g. a column value such as an NSNumber, NSNull). sqlite values are wrapped with
 *  double-quotes.
 */
+ (NSString *)escapeValue:(id)value;

// ========== UPDATES ==================================================================================================
#pragma mark - Updates

/// @name Executing Statements

/**
 *  Executes a database update, or provides an error.
 *
 *  @param  sql             The SQL statement to execute.
 *  @param  error_p         A pointer to any error that occurs.
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)executeUpdate:(NSString *)sql
                error:(NSError **)error_p;

/**
 *  Executes a database update with the given arguments, or provides an error. "?" in the statement will be bound to
 *  arguments in the array.
 *
 *  @param  sql             The SQL statement to execute.
 *  @param  parameters      The ordered arguments to bind to the statement.
 *  @param  error_p         A pointer to any error that occurs.
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)executeUpdate:(NSString *)sql
 withArgumentsInArray:(NSArray *)arguments
                error:(NSError **)error_p;

/**
 *  Executes a statement that updates the database with the given paramters bound. "$key" values in the statement will
 *  be bound to entries in the dictionary.
 *
 *  @param  sql             The SQL statement to execute.
 *  @param  parameters      The named parameters to bind to the statement.
 *  @param  error_p         A pointer to any error that occurs.
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)  executeUpdate:(NSString *)sql
withParameterDictionary:(NSDictionary *)parameters
                  error:(NSError **)error_p;

// ========== SCHEMA ===================================================================================================
#pragma mark - Schema

/// @name Tables

/***
 *  Queries the entire database schema.
 *
 *  @return A dictionary whose keys are table names, and whose values are dictionaries matching the output of
 *          `-tableSchema:`
 */
- (NSDictionary *)databaseSchema;

/**
 *  Queries the schema for a table.
 *
 *  @param tableName        The table's name
 *
 *  @return The schema of the given table.
 */
- (NSDictionary *)tableSchema:(NSString *)tableName;

/**
 *  Queries the names of all tables in the database.
 *
 *  @return The names of a tables in the database.
 */
- (NSSet *)tableNames;

/**
 *  Creates a database table.
 *
 *  @param  tableName       The table's name
 *  @param  columns         SQL column definitions, e.g. "INTEGER PRIMARY KEY"
 *  @param  constraints     Table constraints, e.g. "UNIQUE (emailAddress)
 *  @param  error_p         A pointer to any error that occurs.
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)createTableWithName:(NSString *)tableName
                    columns:(NSArray *)columns
                constraints:(NSArray *)constraints
                      error:(NSError **)error_p;

/**
 *  Renames a database table.
 *
 *  @param  tableName       The name of the table to rename.
 *  @param  newTableName    The new name of the table.
 *  @param  error_p         A pointer to any error that occurs.
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)renameTable:(NSString *)tableName
                 to:(NSString *)newTableName
              error:(NSError **)error_p;

/**
 *  Adds a column to a database table.
 *
 *  @param  columnDefinition  The column to add. E.g. `age INTEGER`
 *  @param  tableName         The name of the table to drop.
 *  @param  error_p           A pointer to any error that occurs
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)addColumn:(NSString *)columnDefinition
          toTable:(NSString *)tableName
            error:(NSError **)error_p;

/**
 *  Removes a table.
 *
 *  @param  tableName         The name of the table to drop.
 *  @param  error_p           A pointer to any error that occurs
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)dropTableWithName:(NSString *)tableName
                    error:(NSError **)error_p;

/**
 *  Removes a table if it exists.
 *
 *  @param  tableName         The name of the table to drop.
 *  @param  error_p           A pointer to any error that occurs
 *
 *  @return `YES` if successful, `NO` if not.
 */
- (BOOL)dropTableIfExistsWithName:(NSString *)tableName
                            error:(NSError **)error_p;

// ========== INDEXES ==================================================================================================
#pragma mark - Indexes

/// @name Indexes

/**
 *  Returns the names of all indexes on a given table.
 */
- (NSArray *)indexNamesOnTable:(NSString *)tableName;

/**
 *  Creates a non-unique index on a table.
 *
 *  @param  indexName   The name of the index
 *  @param  tableName   The table to create the index on
 *  @param  columns     The columns to index.
 *  @param  error_p     A pointer to any error that occurs
 */
- (BOOL)createIndexWithName:(NSString *)indexName
                  tableName:(NSString *)tableName
                    columns:(NSArray *)columns
                      error:(NSError **)error_p;

/**
 *  Creates a unique index on a table.
 *
 *  @param  indexName   The name of the index
 *  @param  tableName   The table to create the index on
 *  @param  columns     The columns to index.
 *  @param  error_p     A pointer to any error that occurs
 */
- (BOOL)createUniqueIndexWithName:(NSString *)indexName
                        tableName:(NSString *)tableName
                          columns:(NSArray *)columns
                            error:(NSError **)error_p;

/**
 *  Removes an index.
 *
 *  @param  indexName   The name of the index to remove.
 *  @param  error_p     A pointer to any error that occurs
 */
- (BOOL)dropIndexWithName:(NSString *)indexName
                    error:(NSError **)error_p;

// ========== INSERT ===================================================================================================
#pragma mark - Insert

/// @name Inserting Rows

/**
 *  Inserts one or more rows into a table.
 *
 *  @param  tableName   The table into which data will be inserted
 *  @param  columns     The names of the column values that will be inserted. E.g. `@[ @"firstName", @"lastName" ]`.
 *  @param  values      An array of arrays of values to insert. Each array must match the columns in name and order.
 *                      For example `@[ @[ @"James", @"Dean" ], @[ @"Marilyn", @"Monroe" ] ]`.
 *  @param  error_p     A pointer to any error that occurs
 */
- (BOOL)insertInto:(NSString *)tableName
           columns:(NSArray *)columns
            values:(NSArray *)values
             error:(NSError **)error_p;

/**
 *  Inserts a row into a table, from a dictionary of values.
 *
 *  @param  tableName   The table into which data will be inserted
 *  @param  rowValues   A dictionary representing the row to insert. For example `@{ @"firstName": @"James" }`.
 *  @param  error_p     A pointer to any error that occurs
 *
 *  @return The IDs of all inserted rows, in the same order as the `dictionaries` array.
 */
- (NSNumber *)insertInto:(NSString *)tableName
                     row:(NSDictionary *)rowValues
                   error:(NSError **)error_p;

// ========== COUNT ====================================================================================================
#pragma mark - Count

/// @name Counting Rows

/**
 *  Count all rows in a table.
 *
 *  @param  from        The table name, plus any joins.
 *  @param  error_p     A pointer to any error that occurs.
 *
 *  @return The count of rows, or -1 if an error occurs.
 */
- (NSInteger)countFrom:(NSString *)from
                 error:(NSError **)error_p;

/**
 *  Count all rows in a table that match conditions.
 *
 *  @param  columnNames The columns (or expressions) to count. `nil` counts all columns.
 *  @param  from        The table name, plus any joins.
 *  @param  where       An optional where clause. E.g. @"firstName IS NOT ?".
 *  @param  arguments   Arguments to bind to the where clause. E.g. @[@"James"].
 *  @param  error_p     A pointer to any error that occurs.
 *
 *  @return The count of rows, or -1 if an error occurs.
 */
- (NSInteger)count:(NSArray *)columnNames
              from:(NSString *)from
             where:(NSString *)where
         arguments:(NSArray *)arguments
             error:(NSError **)error_p;

/**
 *  Count all rows in a table that match conditions.
 *
 *  @param  columnNames The columns (or expressions) to count. `nil` counts all columns.
 *  @param  from        The table name, plus any joins.
 *  @param  where       An optional WHERE clause. E.g. @"firstName IS NOT $excludedName"
 *  @param  parameters  Named parameters to bind to the WHERE clause. E.g. @{ @"excludedName": @"James" }
 *  @param  error_p     A pointer to any error that occurs.
 *
 *  @return The count of rows, or -1 if an error occurs.
 */
- (NSInteger)count:(NSArray *)columnNames
              from:(NSString *)from
             where:(NSString *)where
        parameters:(NSDictionary *)parameters
             error:(NSError **)error_p;

// ========== SELECT ===================================================================================================
#pragma mark - Select

/// @name Selecting Rows

/**
 *  Fetches all rows from a table and returns them as `NSDictionary` instances.
 *  
 *  @param  from        The table name, plus any joins.
 *  @param  orderBy     An optional ordering clause.
 *  @param  error_p     A pointer to any error that occurs.
 *
 *  @return An array of each selected row, as an `NSDictionary`, or `nil` if an error occurs.
 */
- (NSArray *)selectAllFrom:(NSString *)from
                   orderBy:(NSString *)orderBy
                     error:(NSError **)error_p;

/**
 *  Fetches rows from a table that match `where` and returns them as `NSDictionary` instances.
 *
 *  @param  from        The table name, plus any joins.
 *  @param  where       An optional WHERE clause.
 *  @param  arguments   Optional arguments to the WHERE clause. Use '?' in `where` to bind them.
 *  @param  orderBy     An optional ORDER BY clause.
 *  @param  error_p     A pointer to any error that occurs.
 *
 *  @return An array of each selected row, as an `NSDictionary`, or `nil` if an error occurs.
 */
- (NSArray *)selectAllFrom:(NSString *)from
                     where:(NSString *)where
                 arguments:(NSArray *)arguments
                   orderBy:(NSString *)orderBy
                     error:(NSError **)error_p;

// ---------- RESULTS --------------------------------------------------------------------------------------------------
#pragma mark Results

/**
 *  Fetches rows from a table and returns them as an `FMResultSet`.
 *
 *  @param  from        The table name, plus any joins.
 *  @param  orderBy     An optional ORDER BY clause.
 *  @param  error_p     A pointer to any error that occurs
 *
 *  @return The selected rows, or `nil` if an error occurs.
 */
- (FMResultSet *)selectResultsFrom:(NSString *)from
                           orderBy:(NSString *)orderBy
                             error:(NSError **)error_p;

/**
 *  Fetches rows from a table and returns them as an `FMResultSet`.
 *
 *  @param  from        The table name, plus any joins.
 *  @param  where       An optional WHERE clause.
 *  @param  arguments   Arguments to the statement. Use '?' to bind them.
 *  @param  orderBy     An optional ORDER BY clause.
 *  @param  error_p     A pointer to any error that occurs
 *
 *  @return The selected rows, or `nil` if an error occurs.
 */
- (FMResultSet *)selectResultsFrom:(NSString *)from
                             where:(NSString *)where
                         arguments:(NSArray *)arguments
                           orderBy:(NSString *)orderBy
                             error:(NSError **)error_p;

/**
 *  Fetches rows from a table that match the given values and returns them as an `FMResultSet`. `valuesToMatch` contains
 *  column values keyed by the column name. Values can be strings, numbers, and NSArray instances. Only rows that match
 *  all values will be returned. When an array value is provided, rows that match any of the given values are returned.
 *
 *  For example, the following returns all people named "Amelia" with any of the given titles (Mrs., Ms., or Mme.).
 *
 *        [db selectResultsFrom:@"people"
 *               matchingValues:@{ @"title":      @[ @"Mrs.", @"Ms.", @"Mme." ],
 *                                 @"firstName":  @"Amelia" }
 *                      orderBy:@"firstName"
 *                        error:&error]
 *
 *
 *  @param  from          The table name, plus any joins.
 *  @param  valuesToMatch A dictionary of values to match, keyed by the column names.
 *  @param  orderBy       An optional ORDER BY clause.
 *  @param  error_p       A pointer to any error that occurs
 *
 *  @return The selected rows, or `nil` if an error occurs.
 */
- (FMResultSet *)selectResultsFrom:(NSString *)from
                    matchingValues:(NSDictionary *)valuesToMatch
                           orderBy:(NSString *)orderBy
                             error:(NSError **)error_p;

/**
 *  Fetches rows from a table and returns them as an `FMResultSet`.
 *
 *  @param  columns     The columns to select. `nil` selects all columns (*).
 *  @param  from        The table name, plus any joins.
 *  @param  where       An optional WHERE clause.
 *  @param  groupBy     An optional GROUP BY clause.
 *  @param  having      An optional HAVING clause.
 *  @param  arguments   Arguments to the statement. Use '?' to bind them.
 *  @param  orderBy     An optional ORDER BY clause.
 *  @param  limit       An optional LIMIT clause.
 *  @param  offset      An optional OFFSET clause.
 *  @param  error_p     A pointer to any error that occurs
 *
 *  @return The selected rows, or `nil` if an error occurs.
 */
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

/// @name Updating Rows

/**
 *  Updates rows in a table to a set of new values that match the WHERE clause.
 *
 *  @param  tableName     The table to update.
 *  @param  values        The new values. Keys are the column names. E.g. @{ @"firstName": @"Grace" }. Values will be
 *                        escaped.
 *  @param  where         An optional WHERE clause. If `nil` all rows will be updated.
 *  @param  arguments     Arguments to the `where` clause.
 *  @param  error_p       A pointer to any error that occurs
 *
 *  @return The number of updated rows, or -1 if an error occurs.
 */
- (NSInteger)update:(NSString *)tableName
             values:(NSDictionary *)values
              where:(NSString *)where
          arguments:(NSArray *)arguments
              error:(NSError **)error_p;

/**
 *  Updates rows in a table that match the WHERE clause. Unlike `-update:values:where:arguments:error:`, the values
 *  of `set` are not escaped. This allows the values to contain SQL expressions, and arguments to be bound. For example,
 *  `@{ @"firstName", @"lower(firstName)", @"age": @"?" }`, which lowercases the "firstName" column, and sets the "age"
 *  column to a bound argument.
 *
 *  @param  tableName     The table to update.
 *  @param  columnNames   The columns to update.
 *  @param  expressions   The column value expressions. For example @[ @"lower(firstName)", @"?" ].
 *  @param  where         An optional WHERE clause. If `nil` all rows will be updated.
 *  @param  arguments     Arguments to the `expressions` and `where` clauses.
 *  @param  error_p       A pointer to any error that occurs
 *
 *  @return The number of updated rows, or -1 if an error occurs.
 */
- (NSInteger)update:(NSString *)tableName
            columns:(NSArray *)columnNames
        expressions:(NSArray *)expressions
              where:(NSString *)where
          arguments:(NSArray *)arguments
              error:(NSError **)error_p;

// ========== DELETE ===================================================================================================
#pragma mark - Delete

/// @name Deleting Rows

/**
 *  Deletes rows in a table that match the optional WHERE clause.
 *
 *  @param  tableName     The table to update.
 *  @param  where         An optional WHERE clause. If `nil` all rows will be deleted.
 *  @param  arguments     Arguments to the `where` clause.
 *  @param  error_p       A pointer to any error that occurs
 *
 *  @return The number of updated rows, or -1 if an error occurs.
 */
- (NSInteger)deleteFrom:(NSString *)tableName
                  where:(NSString *)where
              arguments:(NSArray *)arguments
                  error:(NSError **)error_p;

@end
