DROP TRIGGER if exists maxEnrollment;
drop TABLE if exists TriggerWarning;

DELIMITER //
CREATE TABLE TriggerWarning (signal1 int(1)); //
DELIMITER ;

DELIMITER //
CREATE TRIGGER maxEnrollment
    BEFORE UPDATE ON uosoffering
    FOR EACH ROW
    BEGIN
    if new.Enrollment<(0.5*NEW.maxEnrollment)
    then     
        insert into TriggerWarning values(1);  
        end if;
    if new.Enrollment>=(0.5*NEW.maxEnrollment)
    then 
        delete from TriggerWarning; 
    end if;
    end ;
    //
DELIMITER ;