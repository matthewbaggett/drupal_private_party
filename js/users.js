jQuery(document).ready(function(){
    var form = jQuery('form.user_list');

    // When Select All is checked, select all the other fields
    jQuery('input[name=select_all]', form).change(function(){
        if(this.checked){
            jQuery('input.batch', form).attr('checked','checked');
        }else{
            jQuery('input.batch', form).removeAttr('checked');
        }
    });

    // When an input.batch is selected, or select_all is selected, decide if we want to show the batch actions buttons
    // or not.
    jQuery('input.batch, input[name=select_all]', form).change(function(){
        if(jQuery('input.batch:checked', form).length > 0){
            jQuery('.actions-batch', form).show();
        }else{
            jQuery('.actions-batch', form).hide();
        }
    }).change();

    // When you push a button, it should write the rel of the button into the action form field.
    // It should then submit the form.
    jQuery('.button a', form).bind('click', function(event){
        event.preventDefault();
        jQuery('input[name=action]', form).val(jQuery(this).parent().attr('rel'));
        form.submit();
    });
});