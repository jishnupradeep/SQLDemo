DROP PROCEDURE if exists withdraw;


delimiter $$
create procedure withdraw 
(
    in pusername int, 
    in pcourseid varchar(10), 
    in psemester varchar(2), 
    in pyear int,
    out res varchar(100)
)

begin

DECLARE exit handler for sqlexception
BEGIN
 ROLLBACK;
END;

DECLARE exit handler for sqlwarning
BEGIN
 ROLLBACK;
END;

if exists (select * from transcript where studid=pusername and uoscode=pcourseid and  year=pyear and semester=psemester and isnull(grade))
then
	START TRANSACTION;
	delete from transcript where studid=pusername and uoscode=pcourseid and  year=pyear and semester=psemester and isnull(grade);
    update uosoffering set enrollment=enrollment-1 where uoscode=pcourseid and  year=pyear and semester=psemester;
    set res='___Course Withdrawal Successful.___';
    COMMIT;
else
set res='___Course Withdrawal Failed. Check Course Code and try again.___';
end if;            

select res;

end $$
delimiter ;
