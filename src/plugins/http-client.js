export const HttpClient = function() {
  /*** HELPERS ***/
  // const handleError = err => {
  //   console.error(
  //     err.response
  //       ? `${err.response.status} - ${
  //           err.response.statusText
  //         } - ${err.config.method.toUpperCase()} ${err.request.responseURL}`
  //       : err,
  //   );
  //   //TODO: Logging?
  // };

  // const handleResult = (result, callback) => {
  //   if (typeof callback === 'function') {
  //     callback(result.data);
  //   }
  // };

  const xhr = new XMLHttpRequest();

  const handleResult = callback => {
    const DONE = 4;
    const OK = 200;
    if (xhr.readyState === DONE) {
      if (xhr.status === OK) {
        callback(JSON.parse(xhr.responseText), xhr);
      } else {
        callback(null, { xhr });
      }
    }
  };

  const get = (url, callback) => {
    if (typeof callback !== 'function') return;

    xhr.open('GET', `api/${url}`);
    xhr.send();

    xhr.onreadystatechange = () => {
      handleResult(callback);
    };
  };

  // const post = (url, data, callback) => {
  //   http
  //     .post(url, data)
  //     .then(result => handleResult(result, callback))
  //     .catch(handleError);
  // };

  // const put = (url, data, callback) => {
  //   http
  //     .put(url, data)
  //     .then(result => handleResult(result, callback))
  //     .catch(handleError);
  // };

  // const del = (url, callback) => {
  //   http
  //     .delete(url)
  //     .then(result => handleResult(result, callback))
  //     .catch(handleError);
  // };

  /*** END HELPERS ***/

  const getPayloads = callback => {
    get('payloads', callback);
  };

  return {
    // get,
    // post,
    // put,
    // del,
    getPayloads,
  };
};

export default {
  install(instance) {
    instance.prototype.$http = new HttpClient();
  },
};
