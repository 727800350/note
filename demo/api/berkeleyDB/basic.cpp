#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <db.h>
#include <glog/logging.h>

/*
 * api: https://docs.oracle.com/cd/E17276_01/html/api_reference/C/index.html
 */
void usage(const char *prog){
	LOG(INFO) << "usage: " << prog << " -lh";
}

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);
	FLAGS_logtostderr = true;
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;
	FLAGS_stderrthreshold = google::INFO;
	FLAGS_log_dir = "./";

	int opt = 0;
	while((opt = getopt(argc, argv, "lh")) != -1){
		switch(opt){
			case 'l':
				FLAGS_logtostderr = false;
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				usage(argv[0]);
				return -1;
		}
	}

	DB *dbp = NULL;
	int ret = db_create(&dbp, NULL, 0);
	if(ret != 0){
		LOG(ERROR) << "db create error[" << ret << "] " << db_strerror(ret);
		return -1;
	}

	ret = dbp->open(dbp, NULL, "single.db", NULL, DB_BTREE, DB_CREATE, 0);
	if(ret != 0){
		LOG(ERROR) << "db open error[" << ret << "] " << db_strerror(ret);
		return -1;
	}

	DBT key;
	DBT data;
	memset(&key, 0, sizeof(DBT));
	memset(&data, 0, sizeof(DBT));

	const char *fruit = "apple";
	key.data = (void *)fruit;
	key.size = strlen(fruit) + 1;
	int number = 16;
	data.data = &number;
	data.size = sizeof(int);

	ret = dbp->put(dbp, NULL, &key, &data, DB_OVERWRITE_DUP);
	if(ret != 0){
		LOG(ERROR) << "db put error[" << ret << "] " << db_strerror(ret);
		return -1;
	}

	dbp->sync(dbp, 0);
	if(ret != 0){
		LOG(ERROR) << "db sync error[" << ret << "] " << db_strerror(ret);
		return -1;
	}

	memset(&data, 0, sizeof(DBT));
	ret = dbp->get(dbp, NULL, &key, &data, 0);
	if(ret != 0){
		LOG(ERROR) << "db get error[" << ret << "] " << db_strerror(ret);
		return -1;
	}
	LOG(INFO) << "key: " << (char *)key.data << "\tvalue: " << *(int *)data.data;

	dbp->close(dbp, 0);
	if(ret != 0){
		LOG(ERROR) << "db close error[" << ret << "] " << db_strerror(ret);
		return -1;
	}

	google::ShutdownGoogleLogging();
	return 0;
}

