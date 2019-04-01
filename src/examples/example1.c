/// Sample records put/query example
#include <ejdb2.h>

#define RCHECK(rc_)          \
  if (rc_) {                 \
    iwlog_ecode_error3(rc_); \
    return 1;                \
  }

static iwrc exec_visitor(EJDB_EXEC *ctx, const EJDB_DOC doc, int64_t *step) {
  // Print document to stderr
  return jbl_as_json(doc->raw, jbl_fstream_json_printer, stderr, JBL_PRINT_PRETTY);
}

int main() {

  EJDB_OPTS opts = {
    .kv = {
      .path = "example.db",
      .oflags = IWKV_TRUNC
    }
  };
  EJDB db;     // EJDB2 storage handle
  int64_t id;  // Document id placeholder
  JQL q = 0;   // Query instance
  JBL jbl = 0; // Json document

  iwrc rc = ejdb_init();
  RCHECK(rc);

  rc = ejdb_open(&opts, &db);
  RCHECK(rc);

  // First record
  rc = jbl_from_json(&jbl, "{\"name\":\"Bianca\", \"age\":4}");
  RCGO(rc, finish);
  rc = ejdb_put_new(db, "parrots", jbl, &id);
  RCGO(rc, finish);
  jbl_destroy(&jbl);

  // Second record
  rc = jbl_from_json(&jbl, "{\"name\":\"Darko\", \"age\":8}");
  RCGO(rc, finish);
  rc = ejdb_put_new(db, "parrots", jbl, &id);
  RCGO(rc, finish);
  jbl_destroy(&jbl);

  // Now execute a query
  rc =  jql_create(&q, "parrots", "/[age > :age]");
  RCGO(rc, finish);

  EJDB_EXEC ux = {
    .db = db,
    .q = q,
    .visitor = exec_visitor
  };

  // Set query placeholder value
  rc = jql_set_i64(q, "age", 0, 3);
  RCGO(rc, finish);

  // Now execute the query
  rc = ejdb_exec(&ux);

finish:
  if (q) jql_destroy(&q);
  if (jbl) jbl_destroy(&jbl);
  ejdb_close(&db);
  RCHECK(rc);
  return 0;
}

