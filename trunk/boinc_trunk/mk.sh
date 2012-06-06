cd /home/boincadm/boinc_trunk/sched/
make && (

for x in 1 2 3 4 5
do
	cp validate_fast sample_trivial_validator sample_assimilator sample_work_generator delete_file request_file_list make_work get_file show_shmem send_file sched_driver wu_check db_dump census file_deleter feeder db_purge update_stats transitioner trickle_handler /home/boincadm/www/projects/freehal_at_home/bin/
	cp cgi fcgi /home/boincadm/www/projects/freehal_at_home/cgi-bin/
	cp -f validate_fast sample_trivial_validator sample_assimilator sample_work_generator delete_file request_file_list make_work get_file show_shmem send_file sched_driver wu_check db_dump census file_deleter feeder db_purge update_stats transitioner trickle_handler /home/boincadm/www/projects/freehal_at_home/bin/
	cp -f cgi fcgi /home/boincadm/www/projects/freehal_at_home/cgi-bin/
	chmod 0775 /home/boincadm/www/projects/freehal_at_home/cgi-bin/*
	chmod u+s /home/boincadm/www/projects/freehal_at_home/cgi-bin/*
done

)

