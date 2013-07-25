function compute_tree_lasts(group_tree){
    jQuery('li', group_tree).removeClass('last');
    jQuery('li:last-child', group_tree).addClass('last');

}
window.onload = function () {
    var group_tree = jQuery('ul#group_tree');
    compute_tree_lasts(group_tree);
    jQuery('li.label', group_tree).bind('click', function(){
        var gid = jQuery(this).attr('rel');
        var group = jQuery('ul.private-party-tree .group_' + gid);
        if(group.hasClass('collapsed')){
            group
                .addClass('expanded')
                .removeClass('collapsed');
        }else{
            group
                .removeClass('expanded')
                .addClass('collapsed');
        }
    });
    
    jQuery('.button.expand').bind('click', function(){
        jQuery('ul.private-party-tree li.label, ul.tree ul.tree')
            .addClass('expanded')
            .removeClass('collapsed');
    });

    jQuery('.button.contract').bind('click', function(){
        jQuery('ul.private-party-tree li.label, ul.tree ul.tree')
            .addClass('collapsed')
            .removeClass('expanded');
    });
};