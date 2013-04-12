<?php
class private_party_group_user extends active_record {
  protected $_table = "private_party_group_user";

  public $uid;
  public $gid;

	public function get_groups(){
		return private_party_group::search()->where('gid', $this->gid)->exec();
	}

  /**
   * Get the user associated with this private_party_group_user
   *
   * @return user_active_record|false
   */
  public function get_user(){
    return user_active_record::search()->where('uid', $this->uid)->execOne();
  }

  /**
   * Customised save.. This table is just a link table.
   *
   * @return private_party_group_user
   */
  public function save($automatic_reload = true){
    db_merge($this->_table)
      ->key(array('gid' => $this->gid, 'uid' => $this->uid))
      ->execute();
    return $this;
  }
}
