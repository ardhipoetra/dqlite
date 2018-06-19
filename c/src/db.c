#include <assert.h>
#include <stddef.h>

#include <sqlite3.h>

#include "db.h"
#include "dqlite.h"
#include "lifecycle.h"
#include "registry.h"
#include "stmt.h"

void dqlite__db_init(struct dqlite__db *db) {
	assert(db != NULL);

	dqlite__lifecycle_init(DQLITE__LIFECYCLE_DB);

	dqlite__stmt_registry_init(&db->stmts);
}

void dqlite__db_close(struct dqlite__db *db) {
	int rc;

	assert(db != NULL);

	dqlite__stmt_registry_close(&db->stmts);

	if (db->db != NULL) {
		rc = sqlite3_close(db->db);

		/* Since we cleanup all existing db resources, SQLite should
		 * never fail, according to the docs. */
		assert(rc == SQLITE_OK);
	}

	dqlite__lifecycle_close(DQLITE__LIFECYCLE_DB);
}

/* TODO: do some validation of the name (e.g. can't begin with a slash) */
int dqlite__db_open(
	struct dqlite__db *db,
	const char *name,
	int flags,
	const char *vfs)
{
	assert(db != NULL);
	assert(name != NULL);
	assert(vfs != NULL);

	return sqlite3_open_v2(name, &db->db, flags, vfs);
}

int dqlite__db_prepare(struct dqlite__db *db, const char *sql, uint64_t *stmt_id)
{
	int rc;
	struct dqlite__stmt *stmt;
	size_t i;
	const char *tail;

	assert(db != NULL);
	assert(db->db != NULL);

	assert(sql != NULL);

	rc = dqlite__stmt_registry_add(&db->stmts, &stmt, &i);
	if (rc != 0) {
		assert(rc == DQLITE_NOMEM);
		rc = SQLITE_NOMEM;
		goto err_stmt_add;
	}

	assert(stmt != NULL);

	rc = sqlite3_prepare_v2(db->db, sql, -1, &stmt->stmt, &tail);
	if (rc != SQLITE_OK) {
		goto err_stmt_prepare;
	}

	*stmt_id = i;

	return SQLITE_OK;

 err_stmt_prepare:
	dqlite__stmt_registry_del(&db->stmts, i);

 err_stmt_add:
	assert(rc != SQLITE_OK);

	return rc;
}

DQLITE__REGISTRY_METHODS(dqlite__db_registry, dqlite__db);