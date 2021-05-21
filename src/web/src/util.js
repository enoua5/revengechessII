function indexOfElement(el)
{
  let i = 0;
  while( (el = el.previousSibling) != null)
    i++;
  return i;
}

// https://stackoverflow.com/a/51597423/3154580
function getDefaultProperty(tag, prop)
{
  try
  {
    const elem = document.createElement(tag)
    document.body.appendChild(elem)
    const defaultStyles = Object.assign({}, window.getComputedStyle(elem))
    document.body.removeChild(elem)
    return defaultStyles[prop]
  }
  catch(err)
  {
    return err
  }
}
